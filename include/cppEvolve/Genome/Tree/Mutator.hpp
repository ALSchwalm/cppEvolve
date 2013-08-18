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
                    prevNode = currentNode;
                    auto nodeLocation = rand() % currentNode->getChildren().size();
                    currentNode = currentNode->getChildren()[nodeLocation];

                    if (currentNode->getChildren().empty()) {
                        auto& children = prevNode->getChildren();
                        delete currentNode;
                        children[nodeLocation] = factory.createRandomTerminator();
                        tree->value = tree->root->eval();
                        return;
                    }
                }

                auto childValue = rand() % currentNode->getChildren().size();
                auto nodeDepth = currentNode->getChildren()[childValue]->getDepth();
                delete currentNode->getChildren()[childValue];

                currentNode->getChildren()[childValue] = factory.createRandomSubTree(nodeDepth-1);
                tree->value = tree->root->eval();
            }
        }
    }


}

#endif
