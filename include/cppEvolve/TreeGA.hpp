#ifndef TREEGA_H_
#define TREEGA_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"
#include "cppEvolve/Genome/Tree/Crossover.hpp"
#include "cppEvolve/Genome/Tree/Mutator.hpp"

#include <limits.h>

namespace evolve
{
    /*!
     * GA for use with the built-in Tree genome.
     *
     * \tparam rType The type to be accepted and returned by all of the functions in the tree
     * \tparam popSize The maximum number of individuals in the population
     */
    template<typename rType, size_t popSize=100>
    class TreeGA
    {
    public:

        /*!
         * \param _generator A TreeFactory instance used to create the initial population
         * \param _evaluator The fitness function for this GA. Fitness must be expressible as a double
         * \param _crossover The function used to create the next generation via some form of crossing over
         * \param _mutator   Used to alter a particular member of the population.
         *                   This function will be called on mutationRate percent of the population
         * \param _selector  Used to choose the members which "survive" to the next generation.
         *                   The function must leave at least 1 member in the population.
         * \param _generations The number of generation for which to run the GA.
         */
        TreeGA(Tree::TreeFactory<rType> _generator,

                 std::function<double(const Tree::Tree<rType>*)> _evaluator,

                 std::function<Tree::Tree<rType>*(const Tree::Tree<rType>*,
                                                 const Tree::Tree<rType>*)> _crossover,

                 std::function<void(Tree::Tree<rType>*,
                                    const Tree::TreeFactory<rType>&)> _mutator,

                 std::function<void(std::vector<Tree::Tree<rType>*>&,
                                    std::function<double(const Tree::Tree<rType>*)>)> _selector,
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

        virtual ~TreeGA(){}

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
                population.push_back(generator.make());
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
                        mutator(member, generator);
                }

                auto score = evaluator(population[0]);
                if ( score > bestScore)
                {
                    delete bestIndividual;
                    bestIndividual = population[0]->clone();
                    bestScore = score;
                }

                if (generation % logFrequency == 0)
                {
                    std::cout << "Generation(" << generation << ") - Fitness:" << bestScore << std::endl;
                }

            }
            std::cout << "Best: " << *bestIndividual << std::endl;
            std::cout << "Fitness: " << bestScore << std::endl;
        }

        /*!
         * Set the percentage of the population to which the mutation function is applied.
         */
        void setMutationRate(float rate) {mutationRate = rate;}

        /*!
         * Get a reference to the current population of the GA.
         */
        std::array<Tree::Tree<rType>*, popSize>& getPopulation() const {return population;}

        /*!
         * Get the most fit member of the population of all time.
         */
        const Tree::Tree<rType>* getBest() const {return bestIndividual;}

    protected:
        std::vector<Tree::Tree<rType>*> population;
        Tree::TreeFactory<rType> generator;

        Tree::Tree<rType>* bestIndividual = nullptr;  //Historically best individual
        double bestScore = std::numeric_limits<float>::lowest();

        std::function<double(const Tree::Tree<rType>*)> evaluator;

        std::function<Tree::Tree<rType>*(const Tree::Tree<rType>*,
                                        const Tree::Tree<rType>*)> crossover;

        std::function<void(Tree::Tree<rType>*,
                      const Tree::TreeFactory<rType>&)> mutator;

        std::function<void(std::vector<Tree::Tree<rType>*>&,
                           std::function<double(const Tree::Tree<rType>*)>)> selector;


        unsigned long generations;
        float mutationRate = 0.6f;
    };
}

#endif
