
#ifndef LIST1D_EVALUATOR_H_
#define LIST1D_EVALUATOR_H_

namespace evolve
{
    namespace List1DGenome
    {
        namespace Evaluator
        {
            template<typename genome>
            inline float sum(const genome& g)
            {
                float total = 0.0f;
                for(auto allele : g) {
                    total += allele;
                }
                return total;
            }
        }
    }
}

#endif
