/*
 * This file servers as an example of the List1D genome and SimpleGA.
 * The algorithm attempts to find the shortest path connecting the
 * 'cities', 'a', 'b', 'c', and 'd'. Initial, the cities are visited
 * in alphabetic order.
 */

#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"
#include <map>

using namespace evolve;
using namespace evolve::List1DGenome;

//define the 'cities' for this traveling salesman
const std::map<const std::array<char, 2>, unsigned int> distances{
    {{'a', 'b'}, 500},
    {{'a', 'c'}, 1},
    {{'a', 'd'}, 140},
    {{'b', 'c'}, 2},
    {{'b', 'd'}, 1},
    {{'c', 'd'}, 200}
};

//A genome is an ordering of these cities
using Genome = List1DFixed<char, 4>;

//Fitness: Minimize the distance along the path
double fitness(const Genome& ind) {
    double total = 0;
    for(auto i= 0U; i < ind.size()-1; ++i) {
        if (ind[i] < ind[i+1]) {
            total += distances.at({ind[i], ind[i+1]});
        }
        else {
            total += distances.at({ind[i+1], ind[i]});
        }
    }
    return total;
}

int main()
{
    //All members of the population start with route A->B->C->D
    auto generator = []() -> Genome { return {'a', 'b', 'c', 'd'}; };

    SimpleGA<Genome, 50> gaList(
        //Generator: The above generator func
        generator,

        //The above fitness function
        fitness,

        //Crossover: Take a random copy of one parent (no crossing over)
        List1DGenome::Crossover::randomCopy<Genome>,

        //Mutator: Swap two of the cities
        List1DGenome::Mutator::swap<Genome>,

        //Select the top 5 each generation ordered by lowest fitness value
        //(smaller distance is better)
        Selector::top<Genome, 5, Ordering::LOWER>);

    //Evolve for 100 generations, converges on "acbd" or "dbca"
    gaList.run(100);
}
