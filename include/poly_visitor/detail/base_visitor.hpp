#pragma once

#include <boost/any.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit_linearly.hpp>

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
struct base_visitor<boost::mpl::empty_base, Visitable>
{
    virtual boost::any visit(Visitable&) = 0;
    virtual boost::any visit(const Visitable&) = 0;
    virtual ~base_visitor() = default;
};

template<typename... Visitables>
struct base_visitor_hierarchy
    : boost::mpl::inherit_linearly<
          typename boost::mpl::vector<Visitables...>,
          detail::base_visitor<boost::mpl::_1, boost::mpl::_2>
      >::type
{
};
        
}}
