
#ifndef LIST1D_MUTATORS_H_
#define LIST1D_MUTATORS_H_

#include "cppEvolve/utils.hpp"

namespace evolve
{
    namespace List1DGenome
    {
        namespace Mutator
        {

            template<typename genome>
            typename std::enable_if<has_indexing<genome>::value>::type
                swap(genome& g)
            {
                unsigned int s1 = rand() % g.size();
                unsigned int s2 = rand() % g.size();

                std::swap(g[s1], g[s2]);
            }

            template<typename genome>
            typename std::enable_if<!has_indexing<genome>::value>::type
                swap(genome& g)
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
    }
}


#endif
