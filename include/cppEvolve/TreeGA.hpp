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

                 const std::function<float(const Tree::Tree<rType>*)>& _evaluator,

                 const std::function<Tree::Tree<rType>*(const Tree::Tree<rType>*,
                                                 const Tree::Tree<rType>*,
                                                 const Tree::TreeFactory<rType>&)>& _crossover,

                 const std::function<void(Tree::Tree<rType>*,
                                    const Tree::TreeFactory<rType>&)>& _mutator,

                 const std::function<void(std::vector<Tree::Tree<rType>*>&,
                                    std::function<float(const Tree::Tree<rType>*)>)>& _selector,
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

        Tree::Tree<rType>* getRandomIndividual() {
            return population[rand() % population.size()];
        }


        void run()
        {
            for(auto i=0U; i < popSize; ++i)
            {
                population.push_back(generator.make());
            }

            for(auto generation=0U; generation < generations; ++generation)
            {
                selector(population, evaluator);

                while(population.size() < popSize) {
                    population.push_back(crossover(getRandomIndividual(),
                                                   getRandomIndividual(),
                                                   generator));
                }


                for(auto member : population) {
                    if (rand() % 100 > 70)
                        mutator(member, generator);
                }
/*
                std::cout << "Fitness:" << evaluator(population[0]) << std::endl;
                std::cout << *population[0];
                std::cout << std::endl;
*/
                auto score = evaluator(population[0]);
                if ( score > bestScore)
                {
                    delete bestIndividual;
                    bestIndividual = new Tree::Tree<rType>(generator.copySubTree(population[0]->root));
                    bestScore = score;
                }
            }
            std::cout << "Best: " << *bestIndividual << std::endl;
            std::cout << "Fitness: " << bestScore << std::endl;
        }


        void setPopulation(const std::array<Tree::Tree<rType>*, popSize>& _population) {
            population = _population;
        }

        std::array<Tree::Tree<rType>*, popSize>& getPopulation() const {return population;}

    protected:
        std::vector<Tree::Tree<rType>*> population;
        Tree::TreeFactory<rType> generator;

        Tree::Tree<rType>* bestIndividual = nullptr;  //Historically best individual
        float bestScore = std::numeric_limits<float>::lowest();

        const std::function<float(const Tree::Tree<rType>*)>& evaluator;

        const std::function<Tree::Tree<rType>*(const Tree::Tree<rType>*,
                                        const Tree::Tree<rType>*,
                                        const Tree::TreeFactory<rType>&)>& crossover;

        const std::function<void(Tree::Tree<rType>*,
                      const Tree::TreeFactory<rType>&)>& mutator;

        const std::function<void(std::vector<Tree::Tree<rType>*>&,
                           std::function<float(const Tree::Tree<rType>*)>)>& selector;


        unsigned long generations;
    };
}

#endif
