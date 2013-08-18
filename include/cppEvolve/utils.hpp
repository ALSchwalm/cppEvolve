#ifndef UTILS_H_
#define UTILS_H_

#include <type_traits>
#include <functional>
#include <vector>
#include <cassert>

namespace evolve
{

    template<typename T>
    struct count_args;

    template<typename T, typename... U>
    struct count_args<std::function<T(U...)>>
    {
        static const unsigned int value = sizeof...(U);
    };


    template<unsigned int... ints> struct Ints{};

    template<typename intList, bool one>
    struct RangeHelper;

    template<unsigned int... ints>
    struct RangeHelper<Ints<ints...>, true>{
        typedef Ints<ints...> type;
    };

    template<unsigned int... ints, unsigned int head>
    struct RangeHelper<Ints<head, ints...>, false> :
        public RangeHelper<Ints<head-1, head, ints...>, head-1==0>{};

    template<unsigned int bound>
    struct Range : public RangeHelper<Ints<bound>, bound==0>{};


    template <typename T>
    struct unpack_caller
    {
    private:
        template <typename FuncType, typename U, unsigned int... I>
        static T call(FuncType f, const std::vector<U>& args, const Ints<I...>&){
            return f(args[I]->eval()...);
        }

    public:
        template <typename FuncType, typename U>
        static T eval(FuncType f, const std::vector<U>& args){
            assert(args.size() == count_args<FuncType>::value); // just to be sure
            return call(f, args, typename Range<count_args<FuncType>::value-1>::type{});
        }
    };


    template<typename>
    struct sfinae_true : std::true_type{};

    #define HAS_MEMBER(func, name)                                                      \
                                                                                            \
        template<typename T>                                                                \
        static auto name ## _test(int) -> sfinae_true<decltype(std::declval<T>().func)>;    \
                                                                                            \
        template<typename>                                                                  \
        static auto name ## _test(long) -> std::false_type;                                 \
                                                                                            \
        template<typename T>                                                                \
        struct name : decltype(name ## _test<T>(0)){};


    HAS_MEMBER(operator[](std::declval<std::size_t>()), has_indexing);

    HAS_MEMBER(push_back(std::declval<typename T::value_type>()), has_pushback);

    HAS_MEMBER(insert(std::declval<typename T::iterator>(),
                      std::declval<typename T::iterator>(),
                      std::declval<typename T::iterator>()), has_range_insert);

    HAS_MEMBER(insert(std::declval<typename T::iterator>(),
                      std::declval<typename T::value_type>()), has_location_insert);

}
#endif
