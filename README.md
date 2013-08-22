cppEvolve
=========

cppEvolve is a genetic programing library taking advantage of c++11 features.

Building
========

The project is headers only, so no building is required. The project utilizing the headers must be compiled with the -std=c++11 flag in g++, or equivilant in other compilers.

Using the Library
=================

The library's interface is designed to be as simple as possible. The user must select from one of the built-in genome representations, or provide their own with a similar interface.
For example, functions from the ListGenome namespace must be templated on types which have a std::vector or std::array like interface.

Once a type has been chosen to represent the genome for a particular algorithm, in most cases the user only needs to construct a "simpleGA" instance with a few functions.

```C++
    typedef std::vector<int> genome;

    Generator<genome> generator([]{return rand();}, 5);

    SimpleGA<genome, 100> ga(generator,
                             List1DGenome::Evaluator::sum<genome>,
                             List1DGenome::Crossover::singlePoint<genome>,
                             List1DGenome::Mutator::swap<genome>,
                             Selector::top<genome, 30>,
                             1000);

```

The simpleGA class requires 5 arguments:

    Allele Generator - A class which accepts a function returning the allele type, as well as the number of alleles per genome
    Evaluator - A function which accepts an individual (genome type) and returns a double representing the fitness
    Crossover - A function accepting two individuals, returning a new individual composed of the two genomes in some way
    Mutator - A function accepting an individual reference which alters it in some way
    Selector - Accepts the population (a std::vector of individuals) by reference and removes some number of them

    Optional:

    Generations - The number of generations the genetic algorithm should run (defaults to 1000)

