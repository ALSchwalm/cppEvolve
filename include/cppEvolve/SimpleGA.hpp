#ifndef SIMPLEGA_H_
#define SIMPLEGA_H_

#include "cppEvolve/utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

namespace evolve
{
    using std::function;

    template<typename Genome>
    using GeneratorType = std::function<Genome()>;

    template<typename Genome>
    using EvaluatorType = std::function<double(const Genome&)>;

    template<typename Genome>
    using CrossoverType = std::function<Genome(const Genome&,
                                               const Genome&)>;

    template<typename Genome>
    using MutatorType = std::function<void(Genome&)>;

    template<typename Genome>
    using SelectorType = std::function<void(std::vector<Genome>&,
                                            EvaluatorType<Genome>)>;


    template<typename Genome, size_t popSize=100>
    class SimpleGA
    {
    public:
        SimpleGA(GeneratorType<Genome> _generator,
                 EvaluatorType<Genome> _evaluator,
                 CrossoverType<Genome> _crossover,
                 MutatorType<Genome> _mutator,
                 SelectorType<Genome>  _selector) :
            generator(_generator),
            evaluator(_evaluator),
            crossover(_crossover),
            mutator(_mutator),
            selector(_selector)
        {
            srand(time(NULL));
        }

        virtual ~SimpleGA(){}

        virtual void run(unsigned int generations, unsigned int logFrequency=100)
        {
            for(auto i=0U; i < popSize; ++i)
            {
                population.push_back(generator());
            }

            for(auto generation=0U; generation < generations; ++generation)
            {
                auto popSizePostSelection = population.size();

                while(population.size() < popSize) {
                    population.push_back(crossover(population[rand() % popSizePostSelection],
                                                   population[rand() % popSizePostSelection]));
                }


                for(size_t i=0; i < popSize*mutationRate; ++i) {
                auto index = rand()%popSize;
                    mutator(population[index]);
                }

                selector(population, evaluator);

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

        void setPopulation(const std::array<Genome, popSize>& _population) {
            population = _population;
        }

        std::array<Genome, popSize>& getPopulation() const {return population;}

    protected:

        std::vector<Genome> population;
        GeneratorType<Genome>  generator;
        EvaluatorType<Genome> evaluator;
        CrossoverType<Genome> crossover;
        MutatorType<Genome> mutator;
        SelectorType<Genome> selector;

        Genome bestMember;
        double bestScore = std::numeric_limits<float>::lowest();
        float mutationRate = 0.6f;
    };
}

#endif
