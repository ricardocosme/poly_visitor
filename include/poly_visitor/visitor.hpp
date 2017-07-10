#pragma once

#include "poly_visitor/detail/base_visitor.hpp"
#include "poly_visitor/detail/visitor_wrapper.hpp"
#include "poly_visitor/detail/result_of_visitor.hpp"
#include "poly_visitor/detail/match_visitor.hpp"

#include <type_traits>

namespace poly_visitor {

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    std::is_same<
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::poly_visitor_base_visitor
        >::type,
        void
     >::value
>::type
apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    using base_visitor = typename Visitable::poly_visitor_base_visitor;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_wrapper<
        detail::visitor_hierarchy,
        Visitor,
        base_visitor,
        result_type>(visitor);
    visitable.accept(wrapper);
    return;
}

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    !std::is_same<
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::poly_visitor_base_visitor
        >::type,
        void
     >::value,
    typename detail::result_of_unary_visitor<
        Visitor,
        typename Visitable::poly_visitor_base_visitor
    >::type
>::type
apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    using base_visitor = typename Visitable::poly_visitor_base_visitor;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_wrapper<
        detail::visitor_hierarchy,
        Visitor,
        base_visitor,
        result_type>(visitor);
    
    using cast_t = typename std::conditional<
        std::is_move_constructible<result_type>::value,
        result_type&&,
        const result_type&>::type;
    
    return boost::any_cast<cast_t>(visitable.accept(wrapper));
}

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    std::is_same<
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::poly_visitor_base_visitor
        >::type,
        void
    >::value
>::type
apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    using base_visitor = typename Visitable::poly_visitor_base_visitor;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_wrapper<
        detail::visitor_const_hierarchy,
        Visitor,
        base_visitor,
        result_type>(visitor);
    
    visitable.accept(wrapper);
    return;
}
    
template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    !std::is_same<
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::poly_visitor_base_visitor
        >::type,
        void
     >::value,
    typename detail::result_of_unary_visitor<
        Visitor,
        typename Visitable::poly_visitor_base_visitor
    >::type
>::type
apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    using base_visitor = typename Visitable::poly_visitor_base_visitor;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_wrapper<
        detail::visitor_const_hierarchy,
        Visitor,
        base_visitor,
        result_type>(visitor);
    using cast_t = typename std::conditional<
        std::is_move_constructible<result_type>::value,
        result_type&&,
        const result_type&>::type;
    
    return boost::any_cast<cast_t>(visitable.accept(wrapper));
}

template<typename Visitable, typename... Lambdas>
inline void
match(Visitable& visitable, Lambdas&&... lambdas)
{    
    auto visitor = detail::match_visitor<typename std::decay<Lambdas>::type...>
        (std::forward<Lambdas>(lambdas)...);
    
    apply_visitor(std::move(visitor), visitable);
}
    
template<typename Visitor>
struct apply_visitor_delayed
{
    apply_visitor_delayed(Visitor& visitor)
        : visitor(visitor) {}
    
    template<typename Visitable>
    typename detail::result_of_unary_visitor<
        Visitor,
        typename std::decay<Visitable>::type::element_type::poly_visitor_base_visitor
    >::type
    operator()(Visitable&& visitable) const
    {
        return apply_visitor(visitor, *visitable);
    }
    
    template<typename Visitable>
    typename detail::result_of_unary_visitor<
        Visitor,
        typename std::remove_pointer<Visitable>::type::poly_visitor_base_visitor
    >::type
    operator()(Visitable* visitable) const
    {
        return apply_visitor(visitor, *visitable);
    }    
    Visitor& visitor;
};

template<typename Visitor>
inline
apply_visitor_delayed<Visitor> apply_visitor(Visitor& visitor)
{
    return apply_visitor_delayed<Visitor>(visitor);
}
    
}
