#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/TreeGA.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"
#include "cppEvolve/Genome/Tree/Tree.hpp"
#include "cppEvolve/Genome/Tree/Mutator.hpp"
#include "cppEvolve/Genome/Tree/Crossover.hpp"
#include <ctime>
#include <cstdlib>
#include <deque>
#include <array>
#include <map>
#include <set>
#include <list>

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

    double fitness(const Tree::Tree<int>* tree)
    {
        x=0;

        return tree->eval();
    }


}

using namespace functions;

int main()
{

    srand(time(NULL));

    typedef std::vector<int> genome;

    Generator<genome> generator([]{return rand();}, 5);

    SimpleGA<genome, 400> ga(generator,
                             List1DGenome::Evaluator::sum<genome>,
                             List1DGenome::Crossover::singlePoint<genome>,
                             List1DGenome::Mutator::swap<genome>,
                             Selector::top<genome, 30>,
                             1000);

    ga.run();


/*
    Tree::TreeFactory<int> factory(5);
    factory.addNode(sum, "sum");
    factory.addNode(product, "product");
    factory.addTerminator([]{return 5;}, "5");
    factory.addTerminator(getX, "X");

    TreeGA<int, 40> ga(factory,
                 fitness,
                 Tree::Crossover::singlePointCrossover<int>,
                 Tree::Mutator::randomNode<int>,
                 Selector::top<Tree::Tree<int>*, 10>,
                 10000);

    ga.run(100);
*/
    return 0;
}
