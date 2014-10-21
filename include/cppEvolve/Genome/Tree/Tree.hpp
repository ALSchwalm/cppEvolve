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
    namespace tree
    {

        /*!
         * The Node and Terminator classes wrap functions and are used to compose function trees
         * which are the genome of TreeGAs. The BaseNode class is used to hide the signature of the
         * wrapped functions.
         */
        template<typename Rtype>
        class BaseNode
        {
        public:

            ///Destructor: Also destroys all child nodes
            virtual ~BaseNode()
            {
                for (auto child : this->children)
                {
                    delete child;
                }
            };

            ///Evaluate the node by evaluating all child nodes
            virtual Rtype eval() const=0;

            ///Get the node's children
            std::vector<BaseNode<Rtype>*>& getChildren() {
                return children;
            }

            ///Compute the depth of the tree from this node
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

            ///Create a deep copy of the node
            virtual BaseNode<Rtype>* clone() const =0;

            ///Get the number of child nodes
            virtual unsigned int getNumChildren() const =0;

            ///Get the name of the wrapped function
            const std::string& getName() const {return name;}

            unsigned int getID() const {return ID;}

            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const BaseNode<T>& node);

        protected:
            BaseNode(const std::string& _name, unsigned int _id) :
                name(_name),
                ID(_id){}

            std::vector<BaseNode<Rtype>*> children;
            const std::string name;
            const unsigned int ID;
        };


        /*!
         * The node class wraps functions which take a non-zero number of arguments.
         */
        template<typename Genome>
        class Node : public BaseNode<typename Genome::result_type>
        {
        public:
            Node(Genome g, const std::string& _name, unsigned int _id) :
                BaseNode<typename Genome::result_type>(_name, _id),
                val(g) {}

            virtual ~Node(){}

            virtual BaseNode<typename Genome::result_type>* clone() const
            {
                auto node = new Node<Genome>(val, this->name, this->ID);
                for (auto child : this->children)
                {
                    node->children.push_back(child->clone());
                }
                return node;
            }

            /*!
             * Evaluates all of the child nodes, then calls the wrapped function with the results
             */
            virtual typename Genome::result_type eval() const override
            {
                return utils::unpack_caller<typename Genome::result_type>::eval(val, this->children);
            }

            virtual unsigned int getNumChildren() const override
            {
                return utils::count_args<Genome>::value;
            }

        private:
            Genome val;
        };


        /*!
         * The node class wraps functions which take no arguments. They can be constructed from
         * lambdas, and are used to 'terminate' the branches of the function tree Genome.
         */
        template<typename Genome>
        class Terminator: public BaseNode<typename Genome::result_type>
        {
        public:
            static_assert(utils::count_args<Genome>::value == 0, "The number of arguments for a terminator must be 0");

            Terminator(Genome g, const std::string& _name, unsigned int _id) :
                BaseNode<typename Genome::result_type>(_name, _id),
                val(g){}

            virtual ~Terminator(){}

            ///Create a copy of the node
            virtual BaseNode<typename Genome::result_type>* clone() const
            {
                auto node = new Terminator<Genome>(val, this->name, this->ID);
                for (auto child : this->children)
                {
                    node->children.push_back(child->clone());
                }
                return node;
            }

            ///Evaluate the node by calling the wrapped function
            virtual typename Genome::result_type eval() const override {
                return val();
            }

            virtual unsigned int getNumChildren() const override  {
                return 0;
            }

        private:
            Genome val;
        };


        /*!
         * Tree class that is the genome for TreeGA
         */
        template<typename Rtype>
        class Tree
        {
        public:
            Tree(BaseNode<Rtype>* _root) :
                root(_root){}

            ~Tree()
            {
                delete root;
            }

            ///Create a deep copy of the tree
            Tree<Rtype>* clone() const
            {
                return new Tree<Rtype>(root->clone());
            }

            /*!
             * Evaluates the root BaseNode
             */
            Rtype eval() const {
                return root->eval();
            }

            unsigned int getDepth() const {
                return root->getDepth();
            }


            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const Tree<T>& tree);

            BaseNode<Rtype>* root;

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
        template<typename Rtype>
        class TreeFactory
        {
        public:
            TreeFactory(unsigned int _depth=5) :
                depth(_depth),
                currentID(0){}

            ///Register a node function (i.e., a function taking 1 or more arguments)
            template<typename... T>
            void addNode(Rtype(*f)(T...), const std::string& name)
            {
                static_assert(sizeof...(T) > 0, "Node function with 0 arguments should be terminator");
                const auto val = currentID;
                std::function<BaseNode<Rtype>*()> func = [f, name, val]() {     //explicitly capture to avoid bug in gcc
                    return new Node<std::function<Rtype(T...)>>(f, name, val);
                };
                nodes[currentID++] = func;
            }

            ///Register a terminator (i.e., a function taking no arguments)
            void addTerminator(Rtype(*f)(), const std::string& name)
            {
                const auto val = currentID;
                std::function<BaseNode<Rtype>*()> func = [f, name, val]() {
                    return new Terminator<std::function<Rtype()>>(f, name, val);
                };
                terminators[currentID++] = func;
            }

            ///Create a tree with the registered functions
            Tree<Rtype>* make() const
            {
                assert(!terminators.empty() && !nodes.empty());
                auto tree = new Tree<Rtype>(createRandomSubTree(depth));

                return tree;
            }

            ///Create a random node
            ///Node: This node must not be eval'd until it has valid children,
            ///      to get a valid node, call createRandomSubTree
            BaseNode<Rtype>* createRandomNode() const
            {
                auto loc = nodes.begin();
                std::advance(loc, utils::random_uint(nodes.size()));
                return ((*loc).second)();
            }

            ///Create a random terminator
            BaseNode<Rtype>* createRandomTerminator() const
            {
                auto loc = terminators.begin();
                std::advance(loc, utils::random_uint(terminators.size()));
                return ((*loc).second)();
            }

            ///Create a random subtree
            BaseNode<Rtype>* createRandomSubTree(unsigned int depth=5) const {

                BaseNode<Rtype>* root = nullptr;

                if (depth == 0) {
                    root = createRandomTerminator();
                }
                else {
                    root = createRandomNode();
                    for (unsigned int i=0; i < root->getNumChildren(); ++i)
                    {
                        root->getChildren().push_back(createRandomSubTree(depth-1));
                    }
                }
                return root;
            }

        protected:
            unsigned int depth;
            unsigned int currentID;
            std::map<unsigned int, std::function<BaseNode<Rtype>*()>> nodes;
            std::map<unsigned int, std::function<BaseNode<Rtype>*()>> terminators;

        };
    }
}


#endif
