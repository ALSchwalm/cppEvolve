#ifndef CPPEVOLVE_TREE_H_
#define CPPEVOLVE_TREE_H_

#include "cppEvolve/utils.hpp"

namespace evolve
{
    namespace Tree
    {

        template<typename rtype>
        class BaseNode
        {
        public:
            virtual ~BaseNode(){};
            virtual rtype eval()=0;

            std::vector<BaseNode<rtype>*>& getChildren() {
                return children;
            }

            virtual unsigned int getNumChildren() const =0;

            const std::string& getName() const {return name;}

        protected:
            BaseNode(const std::string& _name) :
                name(_name){}
            std::vector<BaseNode<rtype>*> children;
            const std::string name;
        };


        template<typename genome>
        class Node : public BaseNode<typename genome::result_type>
        {
        public:

            typedef typename genome::result_type result;

            Node(genome g, const std::string& _name) :
                BaseNode<typename genome::result_type>(_name),
                val(g)
            {
            }

            virtual ~Node(){}

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

            Terminator(genome g, const std::string& _name) :
                BaseNode<typename genome::result_type>(_name),
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

            BaseNode<rType>* root;

            template<typename T>
            friend std::ostream& operator<< (std::ostream &out, const Tree<T>& tree);
        };

        template<typename T>
        std::ostream& operator<< (std::ostream &out, const Tree<T>& tree)
        {

            out << tree.root->getName() << "(";
            return out;
        }


        template<typename rType>
        class TreeFactory
        {
        public:
            TreeFactory(){}

            template<typename... T>
            void addNode(std::function<rType(T...)> f, const std::string& name)
            {
                static_assert(sizeof...(T) > 0, "Node function with 0 arguments should be terminator");
                nodes.push_back(new Node<std::function<rType(T...)>>(f, name));
            }

            void addTerminator(std::function<rType()> f, const std::string& name)
            {
                terminators.push_back(new Terminator<std::function<rType()>>(f, name));
            }

            Tree<rType>* make()
            {
                assert(!terminators.empty());
                auto tree = new Tree<rType>(getRandomNode());

                for (unsigned int i=0; i < tree->root->getNumChildren(); ++i)
                {
                    BaseNode<rType>* node = getRandomNode();
                    tree->root->getChildren().push_back(node);
                    for (unsigned int i=0; i < node->getNumChildren(); ++i)
                    {
                        tree->root->getChildren().push_back(getRandomTerminator());
                    }
                }

                return tree;
            }

        protected:
            std::vector<BaseNode<rType>*> nodes;
            std::vector<BaseNode<rType>*> terminators;

            BaseNode<rType>* getRandomNode() const {
                return nodes[rand() % nodes.size()];
            }

            BaseNode<rType>* getRandomTerminator() const {
                return terminators[rand() % terminators.size()];
            }

        };
    }
}


#endif
