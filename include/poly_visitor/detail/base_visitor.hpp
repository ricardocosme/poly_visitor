#pragma once

namespace poly_visitor { namespace detail {

template<int I, typename Visitables>
struct base_visitor;

template<typename Visitables>
struct base_visitor<0, Visitables>
{
    using Visitable = typename Visitables::template type<0>;
    virtual void visit(Visitable&) = 0;
    virtual void visit(const Visitable&) = 0;
};

template<int I, typename Visitables>
struct base_visitor : base_visitor<I-1, Visitables>
{
    using Visitable = typename Visitables::template type<I-1>;
    using base_visitor<I-1, Visitables>::visit;
    virtual void visit(Visitable&) = 0;
    virtual void visit(const Visitable&) = 0;
};

}}
