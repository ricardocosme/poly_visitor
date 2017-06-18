#pragma once

#include "poly_visitor/type_list.hpp"
#include "poly_visitor/detail/base_visitor.hpp"
#include "poly_visitor/detail/visitor_wrapper.hpp"
#include "poly_visitor/detail/result_of_visitor.hpp"

#include <boost/any.hpp>
#include <type_traits>

namespace poly_visitor {
    
template<typename... Types>
struct base_visitor : detail::base_visitor<TypeList<Types...>::size,
                                           TypeList<Types...>>
{
    using types = TypeList<Types...>;
};

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    std::is_same<
        typename detail::result_of_unary_visitor<
            typename std::decay<Visitor>::type,
            typename Visitable::base_visitor__>::type,
        void>::value,
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::base_visitor__>::type>::type
apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    using base_visitor = typename Visitable::base_visitor__;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_wrapper<
        Visitor,
        typename Visitable::base_visitor__,
        result_type>(visitor);
    visitable.accept(wrapper);
    return;
}

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    !std::is_same<
        typename detail::result_of_unary_visitor<
            typename std::decay<Visitor>::type,
            typename Visitable::base_visitor__>::type,
        void>::value,
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::base_visitor__>::type>::type
apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    using base_visitor = typename Visitable::base_visitor__;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_wrapper<
        Visitor,
        typename Visitable::base_visitor__,
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
            typename std::decay<Visitor>::type,
            typename Visitable::base_visitor__>::type,
        void>::value,
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::base_visitor__>::type>::type
apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    using base_visitor = typename Visitable::base_visitor__;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_const_wrapper<
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
            typename std::decay<Visitor>::type,
            typename Visitable::base_visitor__>::type,
        void>::value,
        typename detail::result_of_unary_visitor<
            Visitor,
            typename Visitable::base_visitor__>::type>::type
apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    using base_visitor = typename Visitable::base_visitor__;
    using result_type = typename detail::result_of_unary_visitor<
        Visitor, base_visitor>::type;
    
    auto wrapper = detail::visitor_const_wrapper<
        Visitor,
        base_visitor,
        result_type>(visitor);
    using cast_t = typename std::conditional<
        std::is_move_constructible<result_type>::value,
        result_type&&,
        const result_type&>::type;
    
    return boost::any_cast<cast_t>(visitable.accept(wrapper));
}
    
template<typename Visitor>
struct apply_visitor_delayed
{
    apply_visitor_delayed(Visitor& visitor)
        : visitor(visitor) {}
    
    template<typename Visitable>
    typename detail::result_of_unary_visitor<
        Visitor,
        typename std::decay<Visitable>::type::element_type::base_visitor__
    >::type
    operator()(Visitable&& visitable) const
    {
        return apply_visitor(visitor, *visitable);
    }
    
    template<typename Visitable>
    typename detail::result_of_unary_visitor<
        Visitor,
        typename std::remove_pointer<Visitable>::type::base_visitor__
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

#define PURE_VISITABLE(BASE_VISITOR)\
using base_visitor__ = BASE_VISITOR;\
virtual boost::any accept(BASE_VISITOR&) = 0;      \
virtual boost::any accept(BASE_VISITOR&) const = 0;

#define VISITABLE(BASE_VISITOR)\
using base_visitor__ = BASE_VISITOR;\
virtual boost::any accept(BASE_VISITOR& visitor)\
{return visitor.visit(*this);}\
virtual boost::any accept(BASE_VISITOR& visitor) const\
{return visitor.visit(*this);}

