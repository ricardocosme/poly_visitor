#pragma once

#include "poly_visitor/detail/empty_base.hpp"

#include <boost/any.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/bind.hpp>

namespace poly_visitor { namespace detail {

template<typename Base,
         typename Visitable,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor : Base
{
    using UserBaseVisitor::visit;
    boost::any visit(Visitable& o) override
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
    boost::any visit(const Visitable& o) override
    { return boost::any{}; /* shut up the compiler */ }
    virtual ~visitor() = default;
};
        
template<typename Visitable,
         typename VisitorWrapper,
         typename UserBaseVisitor>
struct visitor<empty_base, Visitable, VisitorWrapper, UserBaseVisitor> : UserBaseVisitor
{
    using UserBaseVisitor::visit;
    boost::any visit(Visitable& o) override
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
    boost::any visit(const Visitable& o) override
    { return boost::any{}; /* shut up the compiler */ }
    virtual ~visitor() = default;
};

template<typename Visitables, typename VisitorWrapper, typename BaseVisitor>
struct visitor_hierarchy
    : boost::mp11::mp_fold_q<
        Visitables,
        empty_base,
        boost::mp11::mp_bind<
            visitor,
            boost::mp11::_1,
            boost::mp11::_2,
            VisitorWrapper,
            BaseVisitor
        >
    >
{
};

}}
