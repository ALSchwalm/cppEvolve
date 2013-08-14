
#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"
#include <ctime>
#include <cstdlib>

using namespace evolve;

int foo()
{
    return rand() % 20;
};


int main()
{

    srand(time(NULL));
    typedef List1DGenome::List1D<int> genome;

    Generator<genome> generator(foo, 5);

    SimpleGA<genome, 400> ga(generator,
                             List1DGenome::Evaluator::sum<genome>,
                             List1DGenome::Crossover::singlePoint<genome>,
                             List1DGenome::Mutator::swap<genome>,
                             Selector::top<genome, 30>,
                             10);

    ga.run();

    return 0;
}
