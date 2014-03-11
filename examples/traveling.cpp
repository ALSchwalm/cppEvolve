#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"
#include <map>

using namespace evolve;

//define the 'cities' for this traveling salesman
const std::map<std::string, unsigned int> distances{
    {"ab", 5},
    {"ac", 23},
    {"ad", 14},
    {"bc", 22},
    {"bd", 19},
    {"cd", 2}
};

//A genome is an ordering of these cities
using genome = std::array<char, 4>;

double listFitness(const genome& g) {
    return 0.0f;
}

int main()
{
    auto generator = []() -> genome { return {'a', 'b', 'c', 'd'}; };

    SimpleGA<genome, 100> gaList(generator,
                                 listFitness,
                                 List1DGenome::Crossover::singlePoint<genome>,
                                 List1DGenome::Mutator::swap<genome>,
                                 Selector::top<genome, 30>,
                                 1000);

    gaList.run();

    return 0;
}
