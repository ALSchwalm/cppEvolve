#ifndef CPPEVOLVE_TREE_H_
#define CPPEVOLVE_TREE_H_

#include "cppEvolve/utils.hpp"
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

namespace evolve
{
    namespace Tree
    {

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

            virtual rtype eval()=0;

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

            virtual unsigned int getNumChildren() const =0;

            const std::string& getName() const {return name;}

            unsigned int getID() const {return ID;}

            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const BaseNode<T>& node);

        protected:
            BaseNode(const std::string& _name, unsigned int _id) :
                name(_name),
                ID(_id){}

            BaseNode(const BaseNode<rtype>&)=delete;
            BaseNode<rtype> operator=(const BaseNode<rtype>&)=delete;

            std::vector<BaseNode<rtype>*> children;
            const std::string name;
            const unsigned int ID;
        };


        template<typename genome>
        class Node : public BaseNode<typename genome::result_type>
        {
        public:
            Node(genome g, const std::string& _name, unsigned int _id) :
                BaseNode<typename genome::result_type>(_name, _id),
                val(g)
            {
            }

            virtual ~Node()
            {
            }

            virtual typename genome::result_type eval() override
            {
                std::vector<typename genome::result_type> results;

                std::for_each(this->children.begin(), this->children.end(),
                        [&](BaseNode<typename genome::result_type>* child){results.push_back(child->eval());});

                return unpack_caller<typename genome::result_type>::eval(val, results);
            }

            virtual unsigned int getNumChildren() const override
            {
                return count_args<genome>::value;
            }

        private:
            genome val;
        };


        template<typename genome>
        class Terminator: public BaseNode<typename genome::result_type>
        {
        public:
            static_assert(count_args<genome>::value == 0, "The number of arguments for a terminator must be 0");

            Terminator(genome g, const std::string& _name, unsigned int _id) :
                BaseNode<typename genome::result_type>(_name, _id),
                val(g){}

            virtual ~Terminator(){}

            virtual typename genome::result_type eval() override {
                return val();
            }

            virtual unsigned int getNumChildren() const override  {
                return 0;
            }

        private:
            genome val;
        };


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

            rType eval() const {
                return root->eval();
            }

            unsigned int getDepth() const {
                return root->getDepth();
            }


            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const Tree<T>& tree);

            BaseNode<rType>* root;
        protected:

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


        template<typename rType>
        class TreeFactory
        {
        public:
            TreeFactory() :
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

            Tree<rType>* make() const
            {
                assert(!terminators.empty() && !nodes.empty());
                auto tree = new Tree<rType>(createRandomSubTree());

                return tree;
            }


            BaseNode<rType>* copyNode(const BaseNode<rType>* node) const
            {
                if (nodes.find(node->getID()) != nodes.end())
                {
                    return nodes.at(node->getID())();
                }
                else
                {
                    return terminators.at(node->getID())();
                }
            }

            BaseNode<rType>* copySubTree(BaseNode<rType>* node) const
            {
                auto root = copyNode(node);
                for (auto child : node->getChildren())
                {
                    root->getChildren().push_back(copySubTree(child));
                }
                return root;
            }

            BaseNode<rType>* createRandomNode() const
            {
                auto loc = nodes.begin();
                std::advance(loc, rand() % nodes.size());
                return ((*loc).second)();
            }

            BaseNode<rType>* createRandomTerminator() const
            {
                auto loc = terminators.begin();
                std::advance(loc, rand() % terminators.size());
                return ((*loc).second)();
            }

            BaseNode<rType>* createRandomSubTree(unsigned int depth=5) const {

                BaseNode<rType>* root = nullptr;

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
            unsigned int currentID;
            std::map<unsigned int, std::function<BaseNode<rType>*()>> nodes;
            std::map<unsigned int, std::function<BaseNode<rType>*()>> terminators;

        };
    }
}


#endif
