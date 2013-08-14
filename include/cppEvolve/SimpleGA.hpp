#ifndef SIMPLEGA_H_
#define SIMPLEGA_H_

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

namespace evolve
{

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
            generations(_generations)
        {
            srand(time(NULL));
        }

        genomeType& getRandomIndividual() {
            return population[rand() % population.size()];
        }


        void run()
        {
            for(auto generation=0U; generation < generations; ++generation)
            {
                for(auto i=0U; i < popSize; ++i)
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

#endif
