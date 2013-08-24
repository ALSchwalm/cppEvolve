#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "cppEvolve/utils.hpp"

#include <type_traits>
#include <functional>

namespace evolve
{
    template<typename genome>
    class Generator
    {
    public:
        typedef typename genome::value_type allele;
        Generator(std::function<allele(void)> _gen,
              unsigned int _num) :
            gen(_gen),
            num(_num){}


        genome operator()()
        {
            return eval(typename utils::has_pushback<genome>::type());
        }


    protected:
        std::function<allele(void)> gen;
        unsigned int num;

        genome eval(std::true_type)
        {
            genome g;

            for (auto i=0U; i < num; i++) {
                g.push_back(gen());
            }

            return g;
        }

        genome eval(std::false_type)
        {
            genome g;

            for (auto i=0U; i < g.size(); i++) {
                g[i] = gen();
            }

            return g;
        }

    };
}

#endif
