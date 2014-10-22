#ifndef TREEGA_H_
#define TREEGA_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"
#include "cppEvolve/Genome/Tree/Crossover.hpp"
#include "cppEvolve/Genome/Tree/Mutator.hpp"

#include <limits.h>

namespace evolve {

/*!
 * Defines a GA to use with Tree-like genomes (typically for growing algorithms.
 * @param rType The type to be returned from the functions composing the tree
 * @param PopSize The static size of the population
 */
template<typename Rtype, size_t PopSize=100>
class TreeGA
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
    TreeGA(tree::TreeFactory<Rtype> _generator,

           function<float(const tree::Tree<Rtype>*)> _evaluator,

           function<tree::Tree<Rtype>*(const tree::Tree<Rtype>*,
                                       const tree::Tree<Rtype>*)> _crossover,

           function<void(tree::Tree<Rtype>*,
                         const tree::TreeFactory<Rtype>&)> _mutator,

           function<void(std::vector<tree::Tree<Rtype>*>&,
                         function<double(const tree::Tree<Rtype>*)>)> _selector) :

        generator(_generator),
        evaluator(_evaluator),
        crossover(_crossover),
        mutator(_mutator),
        selector(_selector) {}

    virtual ~TreeGA(){}

    /*!
     * Preform the evolution printing statistics a logFrequency intervals
     */
    virtual void run(unsigned int generations, unsigned int logFrequency=100)
        {
            for(auto i=0U; i < PopSize; ++i)
            {
                population.push_back(generator.make());
            }

            for(auto generation=0U; generation < generations; ++generation)
            {
                selector(population, evaluator);

                auto popSizePostSelection = population.size();

                while(population.size() < PopSize) {
                    population.push_back(crossover(population[random_uint(popSizePostSelection)],
                                                   population[random_uint(popSizePostSelection)]));
                }


                for(size_t i=0; i < PopSize*mutationRate; ++i) {
                    auto index = random_uint(PopSize);
                    mutator(population[index], generator);
                }

                auto score = evaluator(population[0]);
                if (score > bestScore)
                {
                    delete bestIndividual;
                    bestIndividual = population[0]->clone();
                    bestScore = score;
                }

                if (generation % logFrequency == 0)
                {
                    std::cout << "Generation(" << generation << ") - Fitness:"
                              << bestScore << std::endl;
                }

            }
            std::cout << "Best: " << *bestIndividual << std::endl;
            std::cout << "Fitness: " << bestScore << std::endl;
        }


    /*!
     * Set the GA population to pre-created population.
     */
    void setPopulation(const std::array<tree::Tree<Rtype>*,
                       PopSize>& _population) {
        population = _population;
    }

    /*!
     * Set the mutation rate for the GA. Mutation guarantees that at least
     * PopSize * rate individuals (not necessarily distinct) will be mutated
     * each generation.
     */
    void setMutationRate(float rate) {mutationRate = rate;}

    const std::array<tree::Tree<Rtype>*, PopSize>& getPopulation() const {
        return population;
    }

    /*!
     * Get a pointer to the historically best individual
     */
    const tree::Tree<Rtype>* getBest() const {
        return bestIndividual;
    }

protected:
    std::vector<tree::Tree<Rtype>*> population;
    tree::TreeFactory<Rtype> generator;

    tree::Tree<Rtype>* bestIndividual = nullptr;  //Historically best individual
    double bestScore = std::numeric_limits<float>::lowest();

    function<float(const tree::Tree<Rtype>*)> evaluator;

    function<tree::Tree<Rtype>*(const tree::Tree<Rtype>*,
                                const tree::Tree<Rtype>*)> crossover;

    function<void(tree::Tree<Rtype>*,
                  const tree::TreeFactory<Rtype>&)> mutator;

    function<void(std::vector<tree::Tree<Rtype>*>&,
                  function<double(const tree::Tree<Rtype>*)>)> selector;

    float mutationRate = 0.6f;
};

}

#endif
