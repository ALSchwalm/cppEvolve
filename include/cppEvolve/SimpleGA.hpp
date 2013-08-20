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
                                    std::function<double(const genomeType&)>)> _selector,
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

        virtual ~SimpleGA(){}

        virtual void run(unsigned int logFrequency=100)
        {
            for(auto i=0U; i < popSize; ++i)
            {
                population.push_back(generator());
            }

            for(auto generation=0U; generation < generations; ++generation)
            {
                selector(population, evaluator);

                auto popSizePostSelection = population.size();

                while(population.size() < popSize) {
                    population.push_back(crossover(population[rand() % popSizePostSelection],
                                                   population[rand() % popSizePostSelection]));
                }


                for(auto member : population) {
                    if (rand() % 10000 > mutationRate*10000)
                        mutator(member);
                }


                if (evaluator(population[0]) > bestScore)
                {
                    bestMember = population[0];
                    bestScore = evaluator(population[0]);
                }

                if (generation % logFrequency == 0) {
                    std::cout << "Generation(" << generation << ") - Fitness:" << bestScore << std::endl;
                }
            }
            std::cout << "Best: ";
            for (auto allele : bestMember)
            {
                std::cout << allele << " ";
            }
            std::cout << std::endl << "Fitness: " <<  evaluator(bestMember) << std::endl;
        }

        void setMutationRate(float rate){mutationRate = rate;}

        void setPopulation(const std::array<genomeType, popSize>& _population) {
            population = _population;
        }

        std::array<genomeType, popSize>& getPopulation() const {return population;}

    protected:

        std::vector<genomeType> population;
        Generator<genomeType> generator;
        std::function<double(const genomeType&)> evaluator;
        std::function<genomeType(const genomeType&,
                                 const genomeType&)> crossover;
        std::function<void(genomeType&)> mutator;
        std::function<void(std::vector<genomeType>&,
                           std::function<float(const genomeType&)>)> selector;

        unsigned long generations;

        genomeType bestMember;
        double bestScore = std::numeric_limits<float>::lowest();
        float mutationRate = 0.6f;
    };
}

#endif
