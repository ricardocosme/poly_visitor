#pragma once

#include <boost/any.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/inherit_linearly.hpp>

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
struct visitor<boost::mpl::empty_base, Visitable, VisitorWrapper,
               UserBaseVisitor> : UserBaseVisitor
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
    : boost::mpl::inherit_linearly<
          Visitables,
          detail::visitor<boost::mpl::_1, boost::mpl::_2,
                          VisitorWrapper, BaseVisitor>
      >::type
{
};

}}
