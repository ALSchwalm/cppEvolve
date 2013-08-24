
#ifndef LIST1D_H_
#define LIST1D_H_

#include "Crossover.hpp"
#include "Mutator.hpp"


namespace evolve
{

    /*!
     *  Defines Mutators and Crossover functions for list-like structures including
     *  std::list, vector, deque and array
     */

    namespace List1DGenome
    {
        template <typename T>
        using List1D = std::vector<T>;

        template<typename T, size_t size>
        using List1DFixed = std::array<T, size>;
    }
}


#endif
