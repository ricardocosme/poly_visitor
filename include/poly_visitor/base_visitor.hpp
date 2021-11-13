#pragma once

#include "poly_visitor/detail/base_visitor.hpp"

#include <boost/mp11/list.hpp>

namespace poly_visitor {
    
template<typename... Visitables>
struct base_visitor : detail::base_visitor_hierarchy<Visitables...>
{
    using visitables = boost::mp11::mp_list<Visitables...>;
};

}
