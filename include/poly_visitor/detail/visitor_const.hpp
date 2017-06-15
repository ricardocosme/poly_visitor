#pragma once

#include <boost/any.hpp>

namespace poly_visitor { namespace detail {

template<int Idx,
         typename Visitables,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor_const;

template<typename Visitables,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor_const<0, Visitables, VisitorWrapper, UserBaseVisitor>
    : UserBaseVisitor
{
    using Visitable = typename Visitables::template type<0>;
    using UserBaseVisitor::visit;
    virtual boost::any visit(Visitable& o)
    { return boost::any{}; /* shut up the compiler */ }
    virtual boost::any visit(const Visitable& o)
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
};

template<int Idx,
         typename Visitables,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor_const
    : visitor_const<Idx-1, Visitables, VisitorWrapper, UserBaseVisitor>
{
    using Visitable = typename Visitables::template type<Idx-1>;
    using UserBaseVisitor::visit;
    virtual boost::any visit(Visitable& o)
    { return boost::any{}; /* shut up the compiler */ }
    virtual boost::any visit(const Visitable& o)
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
};

}}
