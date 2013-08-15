
#ifndef LIST1D_CROSSOVER_H_
#define LIST1D_CROSSOVER_H_

#include "cppEvolve/utils.hpp"

#include <cstdlib>
#include <algorithm>

namespace evolve
{
    namespace List1DGenome
    {
        namespace Crossover
        {
            template<typename genome>
            typename std::enable_if<!has_range_insert<genome>::value, genome>::type
            singlePoint(const genome& g1,
                        const genome& g2)
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
            typename std::enable_if<has_range_insert<genome>::value, genome>::type
            singlePoint(const genome& g1,
                        const genome& g2)
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
    }
}


#endif
