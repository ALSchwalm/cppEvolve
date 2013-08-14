
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
    typedef std::vector<int> genome;

    Generator<genome> generator(foo, 5);

    SimpleGA<genome, 400> ga(generator,
                             ListGenome::Evaluator::sum<genome>,
                             ListGenome::Crossover::singlePoint<genome>,
                             ListGenome::Mutator::swap<genome>,
                             Selectors::top<genome, 30>,
                             10);

    ga.run();

    std::cout << has_insert<ListGenome::List1DFixed<int, 5>>::value << std::endl;
    return 0;
}
