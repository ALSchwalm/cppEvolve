#ifndef SIMPLEGA_H_
#define SIMPLEGA_H_

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

namespace evolve
{

    /*!
     * Basic genetic algorithm. It can be used with most non-tree structures.
     *
     */
    template<typename genomeType, size_t popSize=100>
    class SimpleGA
    {
    public:

        /*!
         * \param _generator A Generator instance used to create the initial population
         * \param _evaluator The fitness function for this GA. Fitness must be expressible as a double
         * \param _crossover The function used to create the next generation via some form of crossing over
         * \param _mutator   Used to alter a particular member of the population.
         *                   This function will be called on mutationRate percent of the population
         * \param _selector  Used to choose the members which "survive" to the next generation.
         *                   The function must leave at least 1 member in the population.
         * \param _generations The number of generation for which to run the GA.
         */
        SimpleGA(Generator<genomeType> _generator,
                 std::function<double(const genomeType&)> _evaluator,
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


        /*!
         * Begin the GA. First, the population is seeded with the Generator, until it contains
         * "popSize" individuals. The simulator then loops for "generations". Each generation
         * it selects the members via the selector then uses the crossover function to refill
         * the missing population. The mutation function is applied to "mutationRate" percent of
         * the population. Logging occurs at completion and every logFrequency steps.
         */
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

        /*!
         * Set the percentage of the population to which the mutation function is applied.
         */
        void setMutationRate(float rate){mutationRate = rate;}

        /*!
         * Set the initial population (if using a generator instance is impractical)
         */
        void setPopulation(const std::array<genomeType, popSize>& _population) {
            population = _population;
        }

        /*!
         * Get a reference to the current population of the GA.
         */
        std::array<genomeType, popSize>& getPopulation() const {return population;}

        /*!
         * Get the most fit member of the population of all time.
         */
        const genomeType& getBest() const {return bestMember;}

    protected:

        std::vector<genomeType> population;
        Generator<genomeType> generator;
        std::function<double(const genomeType&)> evaluator;
        std::function<genomeType(const genomeType&,
                                 const genomeType&)> crossover;
        std::function<void(genomeType&)> mutator;
        std::function<void(std::vector<genomeType>&,
                           std::function<double(const genomeType&)>)> selector;

        unsigned long generations;

        genomeType bestMember;
        double bestScore = std::numeric_limits<double>::lowest();
        float mutationRate = 0.6f;
    };
}

#endif
