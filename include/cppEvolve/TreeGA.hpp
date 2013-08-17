#ifndef TREEGA_H_
#define TREEGA_H_

#include "cppEvolve/Genome/Tree/Tree.hpp"

namespace evolve
{

    template<typename rType, size_t popSize=100>
    class TreeGA
    {
    public:
        TreeGA(Tree::TreeFactory<rType> _generator,

                 std::function<float(const Tree::Tree<rType>*)> _evaluator,

                 std::function<Tree::Tree<rType>*(const Tree::Tree<rType>*,
                                                 const Tree::Tree<rType>*,
                                                 const Tree::TreeFactory<rType>&)> _crossover,

                 std::function<void(Tree::Tree<rType>*,
                                    const Tree::TreeFactory<rType>&)> _mutator,

                 std::function<void(std::vector<Tree::Tree<rType>*>&,
                                    std::function<float(const Tree::Tree<rType>*)>)> _selector,
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
                    mutator(member, generator);
                }

                std::cout << "Fitness:" << evaluator(population[0]) << std::endl;
                std::cout << *population[0];
                std::cout << std::endl;
            }
        }


        void setPopulation(const std::array<Tree::Tree<rType>*, popSize>& _population) {
            population = _population;
        }

        std::array<Tree::Tree<rType>*, popSize>& getPopulation() const {return population;}

    protected:
        std::vector<Tree::Tree<rType>*> population;
        Tree::TreeFactory<rType> generator;

        std::function<float(const Tree::Tree<rType>*)> evaluator;

        std::function<Tree::Tree<rType>*(const Tree::Tree<rType>*,
                                        const Tree::Tree<rType>*,
                                        const Tree::TreeFactory<rType>&)> crossover;

        std::function<void(Tree::Tree<rType>*,
                      const Tree::TreeFactory<rType>&)> mutator;

        std::function<void(std::vector<Tree::Tree<rType>*>&,
                           std::function<float(const Tree::Tree<rType>*)>)> selector;


        unsigned long generations;
    };
}

#endif
