#pragma once

#include "poly_visitor/detail/empty_base.hpp"

#include <boost/any.hpp>
#include <boost/mp11/algorithm.hpp>

namespace poly_visitor { namespace detail {

template<typename Base, typename Visitable>
struct base_visitor : Base
{
    using Base::visit;
    virtual boost::any visit(Visitable&) = 0;
    virtual boost::any visit(const Visitable&) = 0;
    virtual ~base_visitor() = default;
};

template<typename Visitable>
struct base_visitor<empty_base, Visitable>
{
    virtual boost::any visit(Visitable&) = 0;
    virtual boost::any visit(const Visitable&) = 0;
    virtual ~base_visitor() = default;
};

template<typename... Visitables>
struct base_visitor_hierarchy
    : boost::mp11::mp_fold<
        boost::mp11::mp_list<Visitables...>,
        empty_base,
        base_visitor
    >
{
};
        
}}
