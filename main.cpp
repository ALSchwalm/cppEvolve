#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"
#include "cppEvolve/TreeGA.hpp"

using namespace evolve;


namespace functions
{
    int x;

    int sum(int x, int y)
    {
        return x+y;
    }

    int product(int x, int y)
    {
        return x*y;
    }

    int getX()
    {
        return functions::x;
    }

    double treeFitness(const Tree::Tree<int>* tree)
    {
        x=0;

        return tree->eval();
    }

    double listFitness(const std::vector<int>& g)
    {
        double total = 0.0f;
        for(auto allele : g) {
            total += allele;
        }
        return total;
    }
}

using namespace functions;

int main()
{

    srand(time(NULL));

    typedef std::vector<int> genome;

    Generator<genome> generator([]{return rand();}, 5);

    SimpleGA<genome, 100> gaList(generator,
                             listFitness,
                             List1DGenome::Crossover::singlePoint<genome>,
                             List1DGenome::Mutator::swap<genome>,
                             Selector::top<genome, 30>,
                             1000);

    gaList.run();

    Tree::TreeFactory<int> factory(5);
    factory.addNode(sum, "sum");
    factory.addNode(product, "product");
    factory.addTerminator([]{return 5;}, "5");
    factory.addTerminator(getX, "X");

    TreeGA<int, 40> gaTree(factory,
                           treeFitness,
                           Tree::Crossover::singlePoint<int>,
                           Tree::Mutator::randomNode<int>,
                           Selector::top<Tree::Tree<int>*, 10>,
                           1000);

    gaTree.setMutationRate(0.02f);
    gaTree.run(100);

    return 0;
}
