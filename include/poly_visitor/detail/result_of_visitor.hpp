#pragma once

#include "poly_visitor/detail/has_visit.hpp"

#include <type_traits>
#include <boost/mpl/count.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/size.hpp>

namespace poly_visitor { namespace detail {
        
template<typename Visitor, typename Visitable>
struct result_of_unary
{
    using type = typename std::result_of<Visitor(Visitable&)>::type;
};

/* Metafunction to discover the return type of Visitor. It complains
 * if not all types are the same.
 */        
template<typename Visitor, typename BaseVisitor>    
struct result_of_unary_visitor
{
    using decayed_visitor = typename std::decay<Visitor>::type;
    
    using visitables = typename BaseVisitor::visitables;
    using first_visitable = typename boost::mpl::front<visitables>::type;

    using _ = typename boost::mpl::fold
        <visitables,
         first_visitable,
         has_visit_assert<decayed_visitor, boost::mpl::_2>>::type;
    
    /* This must be the result of the Visitor. */
    using type = typename result_of_unary<
        decayed_visitor, first_visitable>::type;
    
    using returns = typename boost::mpl::transform<
        visitables,
        result_of_unary<decayed_visitor, boost::mpl::_1>>::type;
    
    static_assert(
        boost::mpl::count<returns, type>::value
        == boost::mpl::size<returns>::type::value,
        "The visitor is mixing return types. Please, check if all visit "\
        "functions have the same return type.");
};

}}
