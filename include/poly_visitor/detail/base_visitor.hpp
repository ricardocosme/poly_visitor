#pragma once

namespace poly_visitor { namespace detail {

template<int I, typename types>
struct base_visitor;

template<typename types>
struct base_visitor<0, types>
{
    using type = typename types::template type<0>;
    virtual void visit(type&) = 0;
    virtual void visit(const type&) = 0;
};

template<int I, typename types>
struct base_visitor : base_visitor<I-1, types>
{
    using type = typename types::template type<I-1>;
    using base_visitor<I-1, types>::visit;
    virtual void visit(type&) = 0;
    virtual void visit(const type&) = 0;
};

}}
