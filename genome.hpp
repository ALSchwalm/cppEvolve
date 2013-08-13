#include <cmath>
#include <vector>
#include <array>
#include <cassert>
#include <functional>
#include <algorithm>
#include <iostream>

namespace evolve
{

    template<typename genome>
    class Generator
    {
    public:
        typedef typename genome::value_type allele;
        
        Generator(std::function<allele(void)> _gen,
                  unsigned int _num) :
            gen(_gen),
            num(_num){}

        genome operator()() {
            genome g;

            for(auto i=0; i < num; i++) {
                g.insert(g.end(), gen());
            }

            return g;
       }

    protected:
        std::function<allele(void)> gen;
        unsigned int num;

    };
    
    namespace Selectors
    {      
        template<typename genome, size_t num>
        inline void top(std::vector<genome>& population,
                        std::function<float(const genome&)> evaluator)
        {
            static_assert( num >=2, "Selector must leave at least 2 individuals in the population");
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
            inline genome singlePoint(const genome& g1,
                                      const genome& g2)
            {
                genome g;

                auto size = std::min(g1.size(), g2.size());
                auto location = rand() % size;

                auto g1location = g1.begin();
                auto g2location = g2.begin();
                
                std::advance(g1location, location);
                std::advance(g2location, location);
                
                g.insert(g.begin(), g1.begin(), g1location);
                g.insert(g.end(), g2location, g2.end());
                return g;
                
            }
        }

        namespace Mutator
        {
            template<typename genome>
            inline void swap(genome& g)
            {
                unsigned int s1 = rand() % g.size();
                unsigned int s2 = rand() % g.size();

                std::swap(g[s1], g[s2]);
            }
        }
    }

    
    template<typename genomeType, size_t popSize=100>
    class SimpleGA
    {
    public:
        SimpleGA(Generator<genomeType> _generator,
                 std::function<float(const genomeType&)> _evaluator,
                 std::function<genomeType(const genomeType&,
                                          const genomeType&)> _crossover,
                 std::function<void(genomeType&)> _mutator,
                 std::function<void(std::vector<genomeType>&,
                                    std::function<float(const genomeType&)>)> _selector,
                 unsigned long _generations = 10000UL) :
            generator(_generator),
            evaluator(_evaluator),
            crossover(_crossover),
            mutator(_mutator),
            selector(_selector),
            generations(_generations){}

        genomeType& getRandomIndividual() {
            return population[rand() % population.size()];
        }
            
        
        void run()
        {
            for(auto generation=0; generation < generations; ++generation)
            {
                for(auto i=0; i < popSize; ++i)
                {
                    population.push_back(generator());
                }

                selector(population, evaluator);

                while(population.size() < popSize) {
                    population.push_back(crossover(getRandomIndividual(),
                                                   getRandomIndividual()));
                }

                for(auto member : population) {
                    mutator(member);
                }

                std::cout << "Fitness:" << evaluator(population[0]) << std::endl;
                for(auto allele : population[0])
                {
                    std::cout << allele << " ";
                }
                std::cout << std::endl;
            }
        }

                        
        void setPopulation(const std::array<genomeType, popSize>& _population) {
            population = _population;
        }

        std::array<genomeType, popSize>& getPopulation() const {return population;}
        
    protected:
        std::vector<genomeType> population;
        Generator<genomeType> generator;
        std::function<float(const genomeType&)> evaluator;
        std::function<genomeType(const genomeType&,
                                 const genomeType&)> crossover;
        std::function<void(genomeType&)> mutator;
        std::function<void(std::vector<genomeType>&,
                           std::function<float(const genomeType&)>)> selector;

        unsigned long generations;        
    };
}

