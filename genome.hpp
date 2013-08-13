
#include <cmath>
#include <vector>
#include <array>
#include <cassert>
#include <functional>
#include <algorithm>

namespace evolve
{    
    namespace Selectors
    {
        template<typename genome, size_t num>
        inline void top(std::vector<genome>& population,
                        std::function<float(const genome&)> evaluator)
        {
/*          assert( !population.empty() );

            std::sort(population, evaluator);
            
            auto start = population.begin();
            std::advance(start, num);
            population.erase(population.begin(), population.remove(start, population.end()));
*/
        }
    }

    namespace ListGenome
    {
        template <typename T>
        using List1D = std::vector<T>;

        template<typename T, size_t size>
        using List1DFixed = std::array<T, size>;

        namespace Evaluator
        {
            template<typename genome>
            inline float sum(const genome& g)
            {
                float total = 0.0f;
                for(auto allele : g) {
                    total += allele;
                }
                return total; 
            }
        }

        namespace Crossover
        {
            template<typename genome>
            inline void singlePoint(genome& g1,
                                    genome& g2)
            {
                
            }
        }
    }

    
    template<typename genomeType, size_t popSize=100>
    class SimpleGA
    {
    public:
        SimpleGA(std::function<float(const genomeType&)> _evaluator,
                 std::function<void(genomeType&,
                                    genomeType&)> _crossover,
                 std::function<void(std::vector<genomeType>&,
                                    std::function<float(const genomeType&)>)> _selector,
                 unsigned long _generations = 10000UL) :
            evaluator(_evaluator),
            crossover(_crossover),
            selector(_selector),
            generations(_generations){}
            
        void run()
        {
        }
        
    protected:
        std::array<genomeType, popSize> population;
        std::function<float(const genomeType&)> evaluator;
        std::function<void(genomeType&,
                           genomeType&)> crossover;
        std::function<void(std::vector<genomeType>&,
                           std::function<float(const genomeType&)>)> selector;

        unsigned long generations;        
    };
}

