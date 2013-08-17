#ifndef TREE_CROSSOVER_H_
#define TREE_CROSSOVER_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"

namespace evolve
{
    namespace Tree
    {
        namespace Crossover
        {
            template<typename T>
            Tree<T>* singlePointCrossover(const Tree<T>* left, const Tree<T>* right, const TreeFactory<T>& factory)
            {
                auto tree = new Tree<T>(factory.copySubTree(left->root)); //Copy left

                auto treeDepth = right->getDepth();
                auto depth = rand() % treeDepth;

                auto sourceNode = right->root;  //Crossover from the right to the left
                BaseNode<T>* prevSourceNode = nullptr;

                for (unsigned int i=0; i < depth; ++i)
                {
                    if (sourceNode->getChildren().empty())
                    {
                        sourceNode = prevSourceNode; //Do not copy terminators
                        break;
                    }
                    prevSourceNode = sourceNode;
                    sourceNode = sourceNode->getChildren()[rand() % sourceNode->getChildren().size()];
                }

                auto currentNode = tree->root;
                BaseNode<T>* prevNode = nullptr;

                for (unsigned int i=0; i < depth; ++i)
                {
                    if (currentNode->getChildren().empty()) {
                        auto& children = prevNode->getChildren();
                        auto location = std::find(children.begin(), children.end(), currentNode);

                        delete currentNode;
                        *location = factory.copySubTree(sourceNode);
                        break;
                    }
                    else {
                        prevNode = currentNode;
                        currentNode = currentNode->getChildren()[rand() % currentNode->getChildren().size()];
                    }
                }

                if (!currentNode->getChildren().empty()) {
                    auto childValue = rand() % currentNode->getChildren().size();
                    delete currentNode->getChildren()[childValue];

                    currentNode->getChildren()[childValue] = factory.copySubTree(sourceNode);
                }

                return tree;
            }

        }
    }
}

#endif
