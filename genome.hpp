

namespace evolve
{

    class BaseGenome
    {
    public:
        virtual void crossover(float rate)=0;
        virtual void mutate(float rate)=0;
        virtual float evaluate()=0;
    };
    
    template<typename T, size_t size>
    lass ListGenome : public BaseGenome
    {
    public:
        ListGenome(std::function<float(T)> _evaluator,
                   std::function<void(T&, T&)> _crossover) :
            evaluator(_evaluator),
            crossover(_crossover){}

        virtual void crossover(float rate) override {}
        virtual void mutate(float rate) override {}
        virtual float evaluator() override {}
        
    protected:
        std::array<T, size> genome;
        std::function<float(T)> evaluator;
        std::function<void(T&, T&)> crossover;
    };

    
    class SimpleGA
    {
    public:
        SimpleGA(BaseGenome* _genome,
                 unsigned long _generations = 10000UL) :
            genome(_genome),
            gernerations(_generations){}

        void run()
        {
            for(auto generation : generations) {
                genome->
        }
        
    protected:
        std::vector<BaseGenome*>  population;
        unsigned long generations;
        
    };
}
        
        
