
#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdlib>

namespace evolve
{

    namespace Selector
    {
        template<typename genome, size_t num>
        inline void top(std::vector<genome>& population,
                        std::function<float(const genome&)> evaluator)
        {
            static_assert( num >=1, "Selector must leave at least 1 individual in the population");
            assert(population.size() >= num );

            std::sort(population.begin(), population.end(),
                      [&](const genome& left,
                          const genome& right)
                      {
                          return evaluator(left) > evaluator(right);
                      });

            auto start = population.begin();
            std::advance(start, num);
            population.erase(start, population.end());
        }
    }
}

#endif
