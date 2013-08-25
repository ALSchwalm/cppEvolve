#ifndef TREE_CROSSOVER_H_
#define TREE_CROSSOVER_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"

namespace evolve
{
    namespace Tree
    {
        /*!
         *  Contains built-in crossover functions for the Tree genome.
         */
        namespace Crossover
        {

            /*!
             *  Selects a random node from one of the subtrees as the "source". Another node is
             *  selected from the other tree of the same height as the first. A new Tree is created
             *  which is a copy of the first but with the selected branch replaced. The height of the
             *  new tree will be the same as the first tree.
             */
            template<typename T>
            Tree<T>* singlePoint(const Tree<T>* left, const Tree<T>* right)
            {
                auto tree = left->clone(); //Copy left

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
                    prevNode = currentNode;
                    auto nextNodeLocation = rand() % currentNode->getChildren().size();
                    currentNode = currentNode->getChildren()[nextNodeLocation];

                    if (sourceNode->getDepth() == currentNode->getDepth())
                    {
                        auto& children = prevNode->getChildren();
                        delete currentNode;
                        children[nextNodeLocation] = sourceNode->clone();
                        break;
                    }
                    else if (currentNode->getChildren().empty()) {
                        break; //This point should never be reached
                    }
                }

                return tree;
            }

        }
    }
}

#endif
