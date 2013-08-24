
#ifndef LIST1D_CROSSOVER_H_
#define LIST1D_CROSSOVER_H_

#include "cppEvolve/utils.hpp"

#include <cstdlib>
#include <algorithm>

namespace evolve
{
    namespace List1DGenome
    {

        /*!
         *  Contains built-in crossover functions for list-like genomes
         */
        namespace Crossover
        {
            namespace details
            {
                template<typename genome>
                genome singlePointHelper(const genome& g1,
                                         const genome& g2,
                                         std::false_type hasRangeInsert)
                {
                    genome g;

                    auto size = std::min(g1.size(), g2.size());
                    auto location = rand() % size;

                    for (auto i=0U; i < size; ++i)
                    {
                        if (i <= location)
                            g[i] = g1[i];
                        else
                            g[i] = g2[i];
                    }

                    return g;
                }


                template<typename genome>
                genome singlePointHelper(const genome& g1,
                                         const genome& g2,
                                         std::true_type hasRangeInsert)
                {
                    genome g;

                    auto size = std::min(g1.size(), g2.size());
                    auto location = rand() % size;

                    auto g1location = g1.begin();
                    auto g2location = g2.begin();

                    std::advance(g1location, location);
                    std::advance(g2location, location);

                    g.insert(g.begin(), g1.begin(), g1location);
                    g.insert(g.end(), g2location, g2.end());
                    return g;
                }
            }

            /*!
             * Selects a random point on each genome. A new genome is created with the elements
             * before the point on one genome, and the alleles after the point on the other genome.
             * This crossover function preserves the number of alleles in the genome.
             */
            template<typename genome>
            genome singlePoint(const genome& g1,
                               const genome& g2)
            {
                return details::singlePointHelper(g1, g2, typename utils::has_range_insert<genome>::type());
            }
        }
    }
}


#endif
