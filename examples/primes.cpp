/*
 * This file serves as an example for the useage of the Tree genome
 * and TreeGA. The algorithm attemps to grow a function which will
 * yield the most primes on consecutive integer input.
 */

#include "cppEvolve/cppEvolve.hpp"
#include "cppEvolve/TreeGA.hpp"

using namespace evolve;

//List some known primes
const std::array<int, 10> primes = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

//Define functions and variables to be used in the trees
namespace nodes
{
    //'x' an input to the grown function
    int x;

    int sum(int x, int y) { return x+y; }
    int product(int x, int y) { return x*y; }
    int difference(int x, int y) {return x-y;}
    int negative(int x) {return -x;}
    int getX() { return nodes::x; }
}

//The fitness of an individual is the number of distince primes yielded by
//consecutive input
double treeFitness(const tree::Tree<int>* tree)
{
    double total = 0;
    std::vector<int> used;
    for(nodes::x=0; nodes::x < static_cast<int>(primes.size()); ++nodes::x) {
        auto val = tree->eval();
        if (std::find(primes.begin(), primes.end(), val) != primes.end() &&
            std::find(used.begin(), used.end(), val) == used.end()) {

            used.push_back(val);
            total += 1;
        }
    }
    return total;
}

using namespace nodes;

int main() {

    //Construct a Tree generator which will return trees with a max-depth of 5
    tree::TreeFactory<int> factory(5);

    //Register the previous functions
    factory.addNode(sum, "sum");
    factory.addNode(product, "product");
    factory.addNode(difference, "difference");
    factory.addNode(negative, "negative");
    factory.addTerminator([]{return 5;}, "5");
    factory.addTerminator(getX, "X");

    //Construct the algorithm
    TreeGA<int, 100> gaTree(
        //Generator: The previously build TreeFactory
        factory,

        //Fitness: number of distince primes yielded by consecutive input
        treeFitness,

        //Crossover: Choose a single node in one parent tree and crossover with
        // a node from the other parent (hight will not increase)
        tree::crossover::singlePoint<int>,

        //Mutation: Replace a random node with a new random subtree
        tree::mutator::randomNode<int>,

        //Selector: Select the 10 most fit members of the population each generation
        selector::top<tree::Tree<int>*, 10>);

    //Set mutation rate to 2%
    gaTree.setMutationRate(0.02f);

    //Run and log statistics every 20 generations, should reach around 7 primes
    gaTree.run(500, 20);
}
