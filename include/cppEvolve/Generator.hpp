#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "cppEvolve/utils.hpp"

#include <type_traits>
#include <functional>

namespace evolve
{

    template<typename genome, typename enabled = void>
    class Generator;

    template<typename genome>
    class Generator<genome, typename std::enable_if<has_pushback<genome>::value>::type>
    {
    public:
        typedef typename genome::value_type allele;
        Generator(std::function<allele(void)> _gen,
              unsigned int _num) :
            gen(_gen),
            num(_num){}

        genome operator()() {
            genome g;

            for (auto i=0U; i < num; i++) {
                g.push_back(gen());
            }

            return g;
        }

    protected:
        std::function<allele(void)> gen;
        unsigned int num;

    };


    template<typename genome>
    class Generator<genome, typename std::enable_if<!has_pushback<genome>::value>::type>
    {
    public:
        typedef typename genome::value_type allele;
        Generator(std::function<allele(void)> _gen) :
            gen(_gen){}

        genome operator()() {
            genome g;

            for (auto i=0U; i < g.size(); i++) {
                g[i] = gen();
            }

            return g;
        }

    protected:
        std::function<allele(void)> gen;
    };
}

#endif
