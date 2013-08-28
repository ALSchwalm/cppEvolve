
#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <cassert>
#include <functional>
#include <set>
#include <algorithm>
#include <type_traits>

namespace evolve
{
    /*!
     * The namespace for the built in population selectors.
     */
    namespace Selector
    {
        namespace details
        {
            template<typename genome, size_t num, typename Comp>
            void topHelper(std::multiset<genome, Comp>& population,
                           std::function<float(const genome&)> evaluator,
                           std::false_type genomeIsPointer)
            {
                auto location = population.begin();
                std::advance(location, num);

                population.erase(location, population.end());
            }


            template<typename genome, size_t num>
            void topHelper(std::multiset<genome, std::function<bool(const genome, const genome)>>& population,
                           std::function<float(const genome)> evaluator,
                           std::true_type genomeIsPointer)
            {
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
        template<typename genome, size_t num>
        void top(std::multiset<typename std::remove_pointer<genome>::type*, std::function<bool(const typename std::remove_pointer<genome>::type*,
                                                                                               const typename std::remove_pointer<genome>::type*)>>& population,
                 std::function<float(const typename std::remove_pointer<genome>::type*)> evaluator)
        {
            static_assert( num >=1, "Selector must leave at least 1 individual in the population");
            assert(population.size() >= num );

            //details::topHelper<genome, num>(population, evaluator, typename std::is_pointer<genome>::type());
        }
    }
}

#endif
