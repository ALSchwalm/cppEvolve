
#ifndef LIST1D_CROSSOVER_H_
#define LIST1D_CROSSOVER_H_

#include "cppEvolve/utils.hpp"

#include <cstdlib>
#include <algorithm>

namespace evolve
{
    namespace list1d
    {

        /*!
         *  Contains built-in crossover functions for list-like genomes
         */
        namespace crossover
        {
            namespace details
            {
                template<typename Genome>
                Genome singlePointHelper(const Genome& g1,
                                         const Genome& g2,
                                         std::false_type)
                {
                    Genome g;

                    auto size = std::min(g1.size(), g2.size());
                    auto location = utils::random_uint(size);

                    for (auto i=0U; i < size; ++i)
                    {
                        if (i <= location)
                            g[i] = g1[i];
                        else
                            g[i] = g2[i];
                    }

                    return g;
                }


                template<typename Genome>
                Genome singlePointHelper(const Genome& g1,
                                         const Genome& g2,
                                         std::true_type)
                {
                    Genome g;

                    auto size = std::min(g1.size(), g2.size());
                    auto location = utils::random_uint(size);

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
             * Selects a random point on each Genome. A new Genome is created with the elements
             * before the point on one Genome, and the alleles after the point on the other Genome.
             * This crossover function preserves the number of alleles in the Genome.
             */
            template<typename Genome>
            Genome singlePoint(const Genome& g1,
                               const Genome& g2)
            {
                return details::singlePointHelper(g1, g2, typename utils::has_range_insert<Genome>::type());
            }

            /*!
             * Crossover by randomly copying one of the parents
             */
            template<typename Genome>
            Genome randomCopy(const Genome& g1,
                              const Genome& g2)
            {
                if (utils::random_uint(2)) { return g1;}
                return g2;
            }

        }
    }
}


#endif
