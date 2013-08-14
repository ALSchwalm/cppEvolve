#ifndef UTILS_H_
#define UTILS_H_

#include <type_traits>

namespace evolve {

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
                      std::declval<typename T::iterator>()), has_insert);

}
#endif
