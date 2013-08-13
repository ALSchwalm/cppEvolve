

#include "cppEvolve.hpp"

using namespace evolve;

int main()
{
    typedef ListGenome::List1D<int> genome;

    SimpleGA<genome, 400> ga(ListGenome::Evaluator::sum<genome>,
                             ListGenome::Crossover::singlePoint<genome>,
                             ListGenome::Mutator::swap<genome>,
                             Selectors::top<genome, 30>,
                             10000UL);

    ga.run();
    
    return 0;
}
