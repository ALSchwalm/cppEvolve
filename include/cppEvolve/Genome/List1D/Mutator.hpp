
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
            namespace details
            {
                template<typename Genome>
                void swapHelper(Genome& g, std::true_type)
                {
                    unsigned int s1 = rand() % g.size();
                    unsigned int s2 = rand() % g.size();

                    std::swap(g[s1], g[s2]);
                }

                template<typename Genome>
                void swapHelper(Genome& g, std::false_type)
                {
                    unsigned int s1 = rand() % g.size();
                    unsigned int s2 = rand() % g.size();

                    auto s1location = g.begin();
                    auto s2location = g.begin();

                    std::advance(s1location, s1);
                    std::advance(s2location, s2);

                    std::swap(s1location, s2location);
                }
            }


            /*!
             *  Swaps two random alleles in the given Genome
             */
            template<typename Genome>
            void swap(Genome& g)
            {
                details::swapHelper(g, typename utils::has_indexing<Genome>::type());
            }

        }
    }
}


#endif
