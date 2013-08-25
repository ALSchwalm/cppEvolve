#ifndef CPPEVOLVE_TREE_H_
#define CPPEVOLVE_TREE_H_

#include "cppEvolve/utils.hpp"
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

namespace evolve
{

    /*!
     * Contains the definition of the Node and Tree classes used in TreeGA to compose function
     * trees at runtime.
     */
    namespace Tree
    {

        /*!
         * The Node and Terminator classes wrap functions and are used to compose function trees
         * which are the genome of TreeGAs. The BaseNode class is used to hide the signature of the
         * wrapped functions.
         */
        template<typename rtype>
        class BaseNode
        {
        public:
            virtual ~BaseNode()
            {
                for (auto child : this->children)
                {
                    delete child;
                }
            };

            virtual rtype eval() const=0;

            std::vector<BaseNode<rtype>*>& getChildren() {
                return children;
            }

            unsigned int getDepth() const {
                unsigned int max = 0;
                for (auto child : children)
                {
                    auto depth = child->getDepth();
                    if (depth > max)
                        max = depth;
                }
                return max+1;
            }

            /*!
             * Create a new copy of the node, without copying the children vector
             */
            virtual BaseNode<rtype>* clone() const =0;

            virtual unsigned int getNumChildren() const =0;

            const std::string& getName() const {return name;}

            /*!
             * Returns the ID of the function within the node. This ID may be used with
             * a TreeFactory to retrieve the function used to create the node.
             */
            unsigned int getID() const {return ID;}

            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const BaseNode<T>& node);

        protected:
            BaseNode(const std::string& _name, unsigned int _id) :
                name(_name),
                ID(_id){}

            std::vector<BaseNode<rtype>*> children;
            const std::string name;
            const unsigned int ID;
        };


        /*!
         * The node class wraps functions which take a non-zero number of arguments.
         */
        template<typename genome>
        class Node : public BaseNode<typename genome::result_type>
        {
        public:
            Node(genome g, const std::string& _name, unsigned int _id) :
                BaseNode<typename genome::result_type>(_name, _id),
                val(g) {}

            virtual ~Node(){}

            virtual BaseNode<typename genome::result_type>* clone() const
            {
                auto node = new Node<genome>(val, this->name, this->ID);
                for (auto child : this->children)
                {
                    node->children.push_back(child->clone());
                }
                return node;
            }

            /*!
             * Evaluates all of the child nodes, then calls the wrapped function with the results
             */
            virtual typename genome::result_type eval() const override
            {
                return utils::unpack_caller<typename genome::result_type>::eval(val, this->children);
            }

            virtual unsigned int getNumChildren() const override
            {
                return utils::count_args<genome>::value;
            }

        private:
            genome val;
        };


        /*!
         * The node class wraps functions which take no arguments. They can be constructed from
         * lambdas, and are used to 'terminate' the branches of the function tree genome.
         */
        template<typename genome>
        class Terminator: public BaseNode<typename genome::result_type>
        {
        public:
            static_assert(utils::count_args<genome>::value == 0, "The number of arguments for a terminator must be 0");

            Terminator(genome g, const std::string& _name, unsigned int _id) :
                BaseNode<typename genome::result_type>(_name, _id),
                val(g){}

            virtual ~Terminator(){}

            virtual BaseNode<typename genome::result_type>* clone() const
            {
                auto node = new Terminator<genome>(val, this->name, this->ID);
                for (auto child : this->children)
                {
                    node->children.push_back(child->clone());
                }
                return node;
            }

            virtual typename genome::result_type eval() const override {
                return val();
            }

            virtual unsigned int getNumChildren() const override  {
                return 0;
            }

        private:
            genome val;
        };


        /*!
         * Tree class that is the genome for TreeGA
         */
        template<typename rType>
        class Tree
        {
        public:
            Tree(BaseNode<rType>* _root) :
                root(_root){}

            ~Tree()
            {
                delete root;
            }

            Tree<rType>* clone() const
            {
                return new Tree<rType>(root->clone());
            }

            /*!
             * Evaluates the root BaseNode
             */
            rType eval() const {
                return root->eval();
            }

            unsigned int getDepth() const {
                return root->getDepth();
            }


            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const Tree<T>& tree);

            BaseNode<rType>* root;

        };

        template<typename T>
        std::ostream& operator<< (std::ostream &out, const Tree<T>& tree)
        {
            out << *tree.root;
            return out;
        }


        template<typename T>
        std::ostream& operator<< (std::ostream &out, const BaseNode<T>& node)
        {
            out << node.name;

            if (!node.children.empty()) {
                out << "(";
                for (unsigned int i=0; i < node.children.size()-1; ++i)
                    out << *(node.children[i]) << ", ";

                out << *node.children[node.children.size()-1] << ")";
            }
            return out;
        }


        /*
         * Factory used to generate random trees for the TreeGA.
         */
        template<typename rType>
        class TreeFactory
        {
        public:
            TreeFactory(unsigned int _height=5) :
                height(_height),
                currentID(0){}

            template<typename... T>
            void addNode(rType(*f)(T...), const std::string& name)
            {
                static_assert(sizeof...(T) > 0, "Node function with 0 arguments should be terminator");
                const auto val = currentID;
                std::function<BaseNode<rType>*()> func = [f, name, val]() {     //explicitly capture to avoid bug in gcc
                    return new Node<std::function<rType(T...)>>(f, name, val);
                };
                nodes[currentID++] = func;
            }

            void addTerminator(rType(*f)(), const std::string& name)
            {
                const auto val = currentID;
                std::function<BaseNode<rType>*()> func = [f, name, val]() {
                    return new Terminator<std::function<rType()>>(f, name, val);
                };
                terminators[currentID++] = func;
            }

            /*!
             * Construct a new random tree of "height" height. Note that this member may not
             * be called until at least one Node and Terminator have been added to the
             * TreeFactory.
             */
            Tree<rType>* make() const
            {
                assert(!terminators.empty() && !nodes.empty());
                auto tree = new Tree<rType>(createRandomSubTree(height));

                return tree;
            }

            /*!
             * Create a new random Node. This is useful for writing mutators for the Tree genome.
             */
            BaseNode<rType>* createRandomNode() const
            {
                auto loc = nodes.begin();
                std::advance(loc, rand() % nodes.size());
                return ((*loc).second)();
            }

            /*!
             * Create a new random Terminator. This is useful for writing mutators for the Tree genome.
             */
            BaseNode<rType>* createRandomTerminator() const
            {
                auto loc = terminators.begin();
                std::advance(loc, rand() % terminators.size());
                return ((*loc).second)();
            }

            /*!
             * Create a random Node with appropriate children to form a tree of "height" height.
             */
            BaseNode<rType>* createRandomSubTree(unsigned int height=5) const {

                BaseNode<rType>* root = nullptr;

                if (height == 0) {
                    root = createRandomTerminator();
                }
                else {
                    root = createRandomNode();
                    for (unsigned int i=0; i < root->getNumChildren(); ++i)
                    {
                        root->getChildren().push_back(createRandomSubTree(height-1));
                    }
                }
                return root;
            }

        protected:
            unsigned int height; /*!< Height of the generated trees */

            /*!
             *  currentID is incremented after each
             *  Node or Terminator is inserted. This allows
             *  the TreeFactory to be certain that a given BaseNode
             *  ID will be in either the nodes map or the terminators
             *  map.
             */
            unsigned int currentID;

            /*!
             * Map mapping IDs to functions which create BaseNode* to Nodes
             */
            std::map<unsigned int, std::function<BaseNode<rType>*()>> nodes;

            /*!
             * Map mapping IDs to functions which create BaseNode* to Nodes
             */
            std::map<unsigned int, std::function<BaseNode<rType>*()>> terminators;

        };
    }
}


#endif
