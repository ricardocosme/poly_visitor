#pragma once

#include "poly_visitor/detail/base_visitor.hpp"

#include <boost/mpl/vector.hpp>

#include <type_traits>

namespace poly_visitor {
    
template<typename... Visitables>
struct base_visitor : detail::base_visitor_hierarchy<Visitables...>
{
    using visitables = boost::mpl::vector<Visitables...>;
};

}
