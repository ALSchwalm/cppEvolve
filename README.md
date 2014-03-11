cppEvolve
=========

cppEvolve is a genetic programming library taking advantage of c++11 features.

Building
========

The project is headers only, so no building is required. The project utilizing the headers must be compiled with the -std=c++11 flag in g++, or equivalent in other compilers. To build the examples, execute `make` in the project root.

Documentation
================

Documentation may be built using Doxygen by executing `doxygen` the project root.

Using the Library
=================

The library's interface is designed to be as simple as possible. The user may select from one of the built-in genome representations, or provide their own with a similar interface. For example, functions from the ListGenome namespace must be templated on types which have a std::vector or std::array like interface.

Once a type has been chosen to represent the genome for a particular algorithm, in most cases the user only needs to construct a "simpleGA" instance with a few functions. For example, the (highly contrived) example  below will grow a list of three elements maximizing the quantity of even numbers. It converges rapidly on {2, 2, 2}. For more complete examples, see the `examples` folder [here][examples].

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
                                 Crossover::singlePoint<Genome>,
                                 Mutator::swap<Genome>,
                                 Selector::top<Genome, 3>);

        ga.run(1000);
    }


[examples]: https://github.com/ALSchwalm/cppEvolve/tree/master/examples


The simpleGA class requires 5 arguments:

- generator - A function which will return instances of Genome to be used in the initial population
- evaluator - A function which returns a double representing the fitness of a member of the population
- crossover - A function which returns a new member of the population by combining two parents
- mutator - A function which will alter a member of the population in some way
- selector - A function which removes the less fit members from the population

After choosing or defining these functions and construction the GA, call `run` to
begin the evolution. The phases of the evolution proceed in the following order:

1. Generation
2. Crossing Over
3. Mutation
4. Selection
5. If any generations remaining, go to 2 otherwise done

The frequency at which statistics of the population are printed may be controlled via the `logFrequency` argument to `run`. Similarly, the mutation rate may be set via the member function `setMutationRate`.

License
=======

This project is licensed under the terms of the MIT license. For more details please see the license file [here][license].

[license]: https://github.com/ALSchwalm/cppEvolve/blob/master/LICENSE
