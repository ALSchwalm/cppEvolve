
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
    namespace selector
    {
        namespace details
        {
            //Sort the population based on the value of Ordering
            // (saves the user from writing a lambda)
            template<typename Genome>
            void orderedSort(std::vector<Genome>& population,
                             const std::function<float(const Genome&)>& evaluator,
                             Ordering ord) {

                if (ord == Ordering::LOWER) {
                    std::sort(population.begin(), population.end(),
                              [&evaluator](const Genome& left,
                                           const Genome& right)
                              {
                                  return evaluator(left) < evaluator(right);
                              });
                }
                else {
                    std::sort(population.begin(), population.end(),
                              [&evaluator](const Genome& left,
                                           const Genome& right)
                              {
                                  return evaluator(left) > evaluator(right);
                              });
                }
            }

            //Top selector for the case that Genome is not a pointer type
            template<typename Genome, size_t num, Ordering ord>
            void topHelper(std::vector<Genome>& population,
                           const std::function<float(const Genome&)>& evaluator,
                           std::false_type)
            {
                orderedSort(population, evaluator, ord);

                auto location = population.begin();
                std::advance(location, num);

                population.erase(location, population.end());
            }

            //Top selector for the case that Genome is a pointer type
            // (i.e. for Tree)
            template<typename Genome, size_t num, Ordering ord>
            void topHelper(std::vector<Genome>& population,
                           const std::function<float(const Genome&)>& evaluator,
                           std::true_type)
            {
                orderedSort(population, evaluator, ord);

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
         * pointer, the individuals not selected are deleted. Ordering determines
         * whether HIGHER or LOWER values are considered more fit.
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
