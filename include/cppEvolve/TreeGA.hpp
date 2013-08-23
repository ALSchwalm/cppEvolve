#ifndef TREEGA_H_
#define TREEGA_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"

#include <limits.h>

namespace evolve
{

    template<typename rType, size_t popSize=100>
    class TreeGA
    {
    public:
        TreeGA(Tree::TreeFactory<rType> _generator,

                 std::function<float(const Tree::Tree<rType>*)> _evaluator,

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


        void setPopulation(const std::array<Tree::Tree<rType>*, popSize>& _population) {
            population = _population;
        }

        void setMutationRate(float rate) {mutationRate = rate;}

        std::array<Tree::Tree<rType>*, popSize>& getPopulation() const {return population;}

        const Tree::Tree<rType>* getBest() const {return bestIndividual;}

    protected:
        std::vector<Tree::Tree<rType>*> population;
        Tree::TreeFactory<rType> generator;

        Tree::Tree<rType>* bestIndividual = nullptr;  //Historically best individual
        double bestScore = std::numeric_limits<float>::lowest();

        std::function<float(const Tree::Tree<rType>*)> evaluator;

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
