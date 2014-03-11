/*
 * This is the example given in the README.md file.
 * The Genome is a list of 3 integers initially containing {1, 2, 3}.
 * The GA will run for 100 generation with a popSize of 100, with
 * fitness equivalent to the quantity of even numbers in the list.
 */

#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/Genome/List1D/List1D.hpp"

using namespace evolve;
using namespace evolve::List1DGenome;
using Genome = List1DFixed<int, 3>;

int main() {
    auto generator = []() -> Genome {return {1, 2, 3};};

    auto fitness = [](const Genome& g) -> double {
        return std::count_if(g.begin(), g.end(),
                             [](int allele) {return allele % 2 == 0;});
    };

    SimpleGA<Genome, 100> ga(generator,
                             fitness,
                             List1DGenome::Crossover::singlePoint<Genome>,
                             List1DGenome::Mutator::swap<Genome>,
                             Selector::top<Genome, 3>);

    ga.run(1000);
}
