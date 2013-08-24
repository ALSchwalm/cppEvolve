
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
        void topHelper(std::vector<genome>& population,
                       std::function<float(const genome&)> evaluator,
                       std::false_type genomeIsPointer)
        {
            static_assert( num >=1, "Selector must leave at least 1 individual in the population");
            assert(population.size() >= num );

            std::sort(population.begin(), population.end(),
                      [&](const genome& left,
                          const genome& right)
                      {
                          return evaluator(left) > evaluator(right);
                      });

            auto location = population.begin();/*!< Detailed description after the member */
            std::advance(location, num);

            population.erase(location, population.end());
        }


        template<typename genome, size_t num>
        void topHelper(std::vector<genome>& population,
                       std::function<float(const genome&)> evaluator,
                       std::true_type genomeIsPointer)
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

            //Assume the pointer points to dynamically allocated memory
            for (auto iter = location; iter != population.end(); ++iter)
            {
                delete *iter;
            }

            population.erase(location, population.end());
        }


        /*!
         * Select the top 'num' individuals from the population. If genome is a
         * pointer, the individuals not selected are deleted.
         */
        template<typename genome, size_t num>
        void top(std::vector<genome>& population,
                 std::function<float(const genome&)> evaluator)
        {
            topHelper<genome, num>(population, evaluator, typename std::is_pointer<genome>::type());
        }


    }
}

#endif
