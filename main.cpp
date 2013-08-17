
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
    int x =100;

    int sum(int x, int y)
    {
        return x + y;
    }

    int average(int x, int y)
    {
        return (x+y)/2;
    }

    int get5()
    {
        return 5;
    };

    int getX()
    {
        return functions::x;
    }

    template<typename T>
    float fitness(const Tree::Tree<T>* tree)
    {
        x=2;
        return tree->eval();
    }


}

using namespace functions;

int main()
{

    srand(time(NULL));
/*
    typedef std::vector<int> genome;

    Generator<genome> generator(get5, 5);

    SimpleGA<genome, 400> ga(generator,
                             List1DGenome::Evaluator::sum<genome>,
                             List1DGenome::Crossover::singlePoint<genome>,
                             List1DGenome::Mutator::swap<genome>,
                             Selector::top<genome, 30>,
                             10);

    ga.run();


*/
    Tree::TreeFactory<int> factory(6);
    factory.addNode(sum, "sum");
    factory.addNode(average, "average");
    factory.addTerminator(get5, "5");
    factory.addTerminator(getX, "X");

    TreeGA<int> ga(factory,
                 fitness<int>,
                 Tree::Crossover::singlePointCrossover<int>,
                 Tree::Mutator::randomNode<int>,
                 Selector::top<Tree::Tree<int>*, 30>,
                 1000);

    ga.run();


/*
    Tree::TreeFactory<int> factory(6);
    factory.addNode(sum, "sum");
    factory.addNode(average, "average");
    factory.addTerminator(get5, "5");
    factory.addTerminator(getX, "X");
    auto tree = factory.make();
    auto tree2 = factory.make();

    std::cout << *tree << std::endl;

    std::cout << "Eval: " << tree->eval() << std::endl;

    auto result = Tree::Crossover::singlePointCrossover(tree, tree2, factory);
    Tree::Mutator::randomNode(result, factory);

    std::cout << *result << std::endl;

    std::cout << "Eval: " << result->eval() << std::endl;
    std::cout << result->getDepth();
    */
    return 0;
}
