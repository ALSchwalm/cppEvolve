
#ifndef LIST1D_H_
#define LIST1D_H_

#include "Crossover.hpp"
#include "Evaluator.hpp"
#include "Mutator.hpp"


namespace evolve
{
    namespace List1DGenome
    {
        template <typename T>
        using List1D = std::vector<T>;

        template<typename T, size_t size>
        using List1DFixed = std::array<T, size>;
    }
}


#endif
