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
    using utils::random_uint;

    ///Function which will return instances of Genome to be used in the initial population
    template<typename Genome>
    using GeneratorType = std::function<Genome()>;

    ///Function which returns a double representing the fitness of a member of the population
    template<typename Genome>
    using EvaluatorType = std::function<double(const Genome&)>;

    ///Function which returns a new member of the population by combining two parents
    template<typename Genome>
    using CrossoverType = std::function<Genome(const Genome&,
                                               const Genome&)>;

    ///Function which will alter a member of the population in some way
    template<typename Genome>
    using MutatorType = std::function<void(Genome&)>;

    ///Function which removes the less fit members from the population
    template<typename Genome>
    using SelectorType = std::function<void(std::vector<Genome>&,
                                            EvaluatorType<Genome>)>;

    /*!
     * Defines a Simple genetic algorithm. That is, an algorithm for which no special
     * behavior is required to copy or modify the genome.
     */
    template<typename Genome, size_t PopSize=100>
    class SimpleGA
    {
    public:

       /*!
        * @param _generator A function which will return instances of Genome to be used in
        *   the initial population
        *
        * @param _evaluator A function which returns a double representing the fitness of a
        *   member of the population
        *
        * @param _crossover A function which returns a new member of the population by
        *   combining two parents
        *
        * @param _mutator A function which will alter a member of the population in some way
        *
        * @param _selector A function which removes the less fit members from the population
        */
        SimpleGA(GeneratorType<Genome> _generator,
                 EvaluatorType<Genome> _evaluator,
                 CrossoverType<Genome> _crossover,
                 MutatorType<Genome> _mutator,
                 SelectorType<Genome>  _selector) :
            generator(_generator),
            evaluator(_evaluator),
            crossover(_crossover),
            mutator(_mutator),
            selector(_selector){}

        virtual ~SimpleGA(){}

        virtual void run(unsigned int generations, unsigned int logFrequency=100)
        {
            //Generation: create the new members
            for(auto i=0U; i < PopSize; ++i)
            {
                population.push_back(generator());
            }

            for(auto generation=0U; generation < generations; ++generation)
            {
                //Crossover: Add missing members
                auto popSizePostSelection = population.size();
                while(population.size() < PopSize) {
                    population.push_back(crossover(population[random_uint(popSizePostSelection)],
                                                   population[random_uint(popSizePostSelection)]));
                }

                //Mutation: Mutate at least rate*popsize members
                for(size_t i=0; i < PopSize*mutationRate; ++i) {
                    auto index = random_uint(PopSize);
                    mutator(population[index]);
                }

                //Selection: Destroy the least fit members
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

        void setPopulation(const std::array<Genome, PopSize>& _population) {
            population = _population;
        }

        std::array<Genome, PopSize>& getPopulation() const {return population;}

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
