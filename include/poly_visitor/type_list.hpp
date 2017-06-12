#pragma once

#include <tuple>

namespace poly_visitor {

template<typename... T>
struct TypeList
{
    template<std::size_t N>
    using type = typename std::tuple_element<N, std::tuple<T...>>::type;
    const static std::size_t size = sizeof...(T);
};

struct EmptyType{};

}
