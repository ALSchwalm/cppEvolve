
#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <type_traits>

namespace evolve
{

    namespace Selector
    {
        template<typename genome, size_t num>
        typename std::enable_if<!std::is_pointer<genome>::value>::type
        top(std::vector<genome>& population,
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

            auto location = population.begin();
            std::advance(location, num);

            population.erase(location, population.end());
        }


        template<typename genome, size_t num>
        typename std::enable_if<std::is_pointer<genome>::value>::type
        top(std::vector<genome>& population,
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

            auto location = population.begin();
            std::advance(location, num);

            for (auto iter = location; iter != population.end(); ++iter)
            {
                delete *iter;
            }

            population.erase(location, population.end());
        }
    }
}

#endif
