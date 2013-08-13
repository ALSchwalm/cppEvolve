
#include "cppEvolve.hpp"
#include <ctime>

using namespace evolve;

int foo()
{
    return rand() % 20;
};


int main()
{
    srand(time(NULL));
    typedef ListGenome::List1D<int> genome;

    Generator<genome> generator(foo, 30);
    
    SimpleGA<genome, 400> ga(generator,
                             ListGenome::Evaluator::sum<genome>,
                             ListGenome::Crossover::singlePoint<genome>,
                             ListGenome::Mutator::swap<genome>,
                             Selectors::top<genome, 30>,
                             10);

    ga.run();
    
    return 0;
}
