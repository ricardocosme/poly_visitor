#pragma once

namespace poly_visitor { namespace detail {

template<int I, typename types, typename DV, typename UserBaseVisitor>
struct visitor_const_visitable;

template<typename types, typename DV, typename UserBaseVisitor>
struct visitor_const_visitable<0, types, DV, UserBaseVisitor> : UserBaseVisitor
{
    using type = typename types::template type<0>;
    using UserBaseVisitor::visit;
    virtual void visit(type& o)
    { /* shut up the compiler */ }
    virtual void visit(const type& o)
    { return static_cast<DV&>(*this).visit(o); }
};

template<int I, typename types, typename DV, typename UserBaseVisitor>
struct visitor_const_visitable : visitor_const_visitable<I-1, types, DV, UserBaseVisitor>
{
    using type = typename types::template type<I-1>;
    using UserBaseVisitor::visit;
    virtual void visit(type& o)
    { /* shut up the compiler */ }
    virtual void visit(const type& o)
    { return static_cast<DV&>(*this).visit(o); }
};

}}
