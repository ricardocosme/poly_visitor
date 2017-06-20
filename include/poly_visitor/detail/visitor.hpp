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
    virtual boost::any visit(Visitable& o)
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
    virtual boost::any visit(const Visitable& o)
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
    virtual boost::any visit(Visitable& o)
    { return static_cast<VisitorWrapper&>(*this).visit(o); }
    virtual boost::any visit(const Visitable& o)
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
