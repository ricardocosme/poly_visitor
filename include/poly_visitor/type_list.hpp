#pragma once

#include <tuple>
#include <boost/mpl/vector.hpp>

namespace poly_visitor {

template<typename... T>
struct TypeList
{
    //TODO:Review this mess.
    using mpl_sequence = boost::mpl::vector<T...>;
    template<std::size_t N>
    using type = typename std::tuple_element<N, std::tuple<T...>>::type;
    const static std::size_t size = sizeof...(T);
};

struct EmptyType{};

}
