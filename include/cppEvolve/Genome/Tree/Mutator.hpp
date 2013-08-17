#ifndef TREE_MUTATORS_H_
#define TREE_MUTATORS_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"

namespace evolve
{
    namespace Tree
    {
        namespace Mutator
        {
            template<typename T>
            void randomNode(Tree<T>* tree, const TreeFactory<T>& factory)
            {
                auto treeDepth = tree->getDepth();
                auto depth = rand() % treeDepth;

                auto currentNode = tree->root;
                BaseNode<T>* prevNode = nullptr;

                for (unsigned int i=0; i < depth; ++i)
                {
                    if (currentNode->getChildren().empty()) {
                        auto& children = prevNode->getChildren();
                        auto location = std::find(children.begin(), children.end(), currentNode);

                        delete currentNode;
                        *location = factory.createRandomNode();
                        return;
                    }
                    else {
                        prevNode = currentNode;
                        currentNode = currentNode->getChildren()[rand() % currentNode->getChildren().size()];
                    }
                }

                if (!currentNode->getChildren().empty()) {
                    auto childValue = rand() % currentNode->getChildren().size();
                    delete currentNode->getChildren()[childValue];

                    currentNode->getChildren()[childValue] = factory.createRandomSubTree(treeDepth - depth);
                }

            }
        }
    }


}

#endif
