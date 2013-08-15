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
        public RangeHelper<Ints<head-1, head, ints...>, head-1==1>{};

    template<unsigned int bound>
    struct Range : public RangeHelper<Ints<bound>, bound==1>{};


    template <typename T>
    struct unpack_caller
    {
    private:
        template <typename FuncType, unsigned int... I>
        static T call(FuncType f, const std::vector<T>& args, Ints<I...>){
            return f(args[I]...);
        }

    public:
        template <typename FuncType>
        static T eval(FuncType f, const std::vector<T>& args){
            assert(args.size() == count_args<FuncType>::value); // just to be sure
            return call(f, args, typename Range<count_args<FuncType>::value>::type{});
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


    HAS_MEMBER(operator[](0), has_indexing);

    HAS_MEMBER(push_back(std::declval<typename T::value_type>()), has_pushback);

    HAS_MEMBER(insert(std::declval<typename T::iterator>(),
                      std::declval<typename T::iterator>(),
                      std::declval<typename T::iterator>()), has_range_insert);

    HAS_MEMBER(insert(std::declval<typename T::iterator>(),
                      std::declval<typename T::value_type>()), has_location_insert);

}
#endif
