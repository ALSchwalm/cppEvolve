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

                delete tree->root;
                tree->root = factory.createRandomSubTree();

            }
        }
    }


}

#endif
