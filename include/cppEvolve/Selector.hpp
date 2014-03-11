
#ifndef SELECTOR_H_
#define SELECTOR_H_

#include "cppEvolve/utils.hpp"
#include <cassert>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace evolve
{
    /*!
     * The namespace for the built-in population selectors.
     */
    namespace Selector
    {

        template<typename Genome>
        std::function<void(const Genome&, const Genome&)>
        sortOrder(std::function<float(const Genome&)> evaluator, Ordering ord) {
            if (ord == Ordering::LOWER) {
                return [&](const Genome& left,
                           const Genome& right)
                {
                    return evaluator(left) < evaluator(right);
                };
            }
            else {
                return [&](const Genome& left,
                           const Genome& right)
                {
                    return evaluator(left) > evaluator(right);
                };
            }
        }

        namespace details
        {
            template<typename Genome, size_t num, Ordering ord>
            void topHelper(std::vector<Genome>& population,
                           const std::function<float(const Genome&)>& evaluator,
                           std::false_type)
            {
                //std::sort(population.begin(), population.end(),
                //sortOrder<Genome>(evaluator, ord));
                if (ord == Ordering::LOWER)
                    std::sort(population.begin(), population.end(),
                              [&](const Genome& left,
                                  const Genome& right)
                              {
                                  return evaluator(left) < evaluator(right);
                              });
                else
                    std::sort(population.begin(), population.end(),
                              [&](const Genome& left,
                                  const Genome& right)
                              {
                                  return evaluator(left) > evaluator(right);
                              });

                auto location = population.begin();
                std::advance(location, num);

                population.erase(location, population.end());
            }


            template<typename Genome, size_t num, Ordering ord>
            void topHelper(std::vector<Genome>& population,
                           const std::function<float(const Genome&)>& evaluator,
                           std::true_type)
            {
                if (ord == Ordering::LOWER)
                    std::sort(population.begin(), population.end(),
                              [&](const Genome& left,
                                  const Genome& right)
                              {
                                  return evaluator(left) < evaluator(right);
                              });
                else
                    std::sort(population.begin(), population.end(),
                              [&](const Genome& left,
                                  const Genome& right)
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
        }

        /*!
         * Select the top 'num' individuals from the population. If genome is a
         * pointer, the individuals not selected are deleted.
         */
        template<typename Genome, size_t num, Ordering order = Ordering::HIGHER>
        void top(std::vector<Genome>& population,
                 const std::function<float(const Genome&)>& evaluator)
        {
            static_assert( num >=1, "Selector must leave at least 1 individual in the population");
            assert(population.size() >= num );

            details::topHelper<Genome, num, order>(population, evaluator, typename std::is_pointer<Genome>::type());
        }


    }
}

#endif
