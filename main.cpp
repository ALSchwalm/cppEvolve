
#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"
#include "cppEvolve/Genome/Tree/Tree.hpp"
#include <ctime>
#include <cstdlib>
#include <deque>
#include <array>
#include <map>
#include <set>
#include <list>

using namespace evolve;


int bar(int x, int y)
{
    return x + y;
}

int get5()
{
    return 5;
};

int get7()
{
    return 7;
}


int main()
{

    srand(time(NULL));
    /*
    typedef std::vector<int> genome;

    Generator<genome> generator(foo, 5);

    SimpleGA<genome, 400> ga(generator,
                             List1DGenome::Evaluator::sum<genome>,
                             List1DGenome::Crossover::singlePoint<genome>,
                             List1DGenome::Mutator::swap<genome>,
                             Selector::top<genome, 30>,
                             10);

    ga.run();
    */
    Tree::TreeFactory<int> factory;
    factory.addNode(std::function<int(int, int)>(bar), "bar");
    factory.addTerminator(std::function<int()>(get5), "5");
    factory.addTerminator(std::function<int()>(get7), "7");

    auto tree = factory.make();

    std::cout << *tree << std::endl;

    std::cout << "Eval: " << tree->eval() << std::endl;

    return 0;
}
