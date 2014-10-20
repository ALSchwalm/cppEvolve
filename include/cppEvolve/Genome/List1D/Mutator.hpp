
#ifndef LIST1D_MUTATORS_H_
#define LIST1D_MUTATORS_H_

#include "cppEvolve/utils.hpp"

namespace evolve
{
    namespace List1DGenome
    {

        /*!
         *  Contains built-in mutators for list-like genomes
         */
        namespace Mutator
        {
            /*!
             *  Swaps two random alleles in the given Genome
             */
            template<typename Genome>
            void swap(Genome& g)
            {
                unsigned int s1 = utils::random_uint(g.size());
                unsigned int s2 = utils::random_uint(g.size());

                auto s1location = begin(g);
                auto s2location = begin(g);

                std::advance(s1location, s1);
                std::advance(s2location, s2);

                std::swap(*s1location, *s2location);
            }

        }
    }
}


#endif
