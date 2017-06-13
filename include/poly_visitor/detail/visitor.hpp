#pragma once

namespace poly_visitor { namespace detail {

template<int Idx,
         typename Visitables,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor;

template<typename Visitables,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor<0, Visitables, VisitorWrapper, UserBaseVisitor>
    : UserBaseVisitor
{
    using Visitable = typename Visitables::template type<0>;
    using UserBaseVisitor::visit;
    virtual void visit(Visitable& o)
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
    virtual void visit(const Visitable& o)
    { /* shut up the compiler */ }
};

template<int Idx,
         typename Visitables,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor
    : visitor<Idx-1, Visitables, VisitorWrapper, UserBaseVisitor>
{
    using Visitable = typename Visitables::template type<Idx-1>;
    using UserBaseVisitor::visit;
    virtual void visit(Visitable& o)
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
    virtual void visit(const Visitable& o)
    { /* shut up the compiler */ }
};

}}
