#pragma once

#include "poly_visitor/detail/has_visit.hpp"

#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/bind.hpp>
#include <boost/mp11/list.hpp>
#include <type_traits>

namespace poly_visitor { namespace detail {
        
template<typename Visitor, typename Visitable>
using result_of_unary = typename std::result_of<Visitor(Visitable&)>::type;

/* Metafunction to discover the return type of Visitor. It complains
 * if not all types are the same.
 */        
template<typename Visitor, typename BaseVisitor>    
struct result_of_unary_visitor
{
    using decayed_visitor = typename std::decay<Visitor>::type;
    
    using visitables = typename BaseVisitor::visitables;
    using first_visitable = boost::mp11::mp_front<visitables>;

    using _ = typename boost::mp11::mp_fold_q<
        visitables,
        first_visitable,
        boost::mp11::mp_bind<has_visit_assert, decayed_visitor, boost::mp11::_2>
    >;
    
    /* This must be the result of the Visitor. */
    using type = result_of_unary<decayed_visitor, first_visitable>;
    
    using returns = boost::mp11::mp_transform_q<
        boost::mp11::mp_bind<result_of_unary, decayed_visitor, boost::mp11::_1>,
        visitables
    >;

    static_assert(
        boost::mp11::mp_count<returns, type>::value == boost::mp11::mp_size<returns>::value,
        "The visitor is mixing return types. Please, check if all visit "\
        "functions have the same return type.");
};

}}
