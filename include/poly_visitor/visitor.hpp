#pragma once

#include "poly_visitor/type_list.hpp"
#include "poly_visitor/detail/base_visitor.hpp"
#include "poly_visitor/detail/visitor_wrapper.hpp"

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
        typename std::decay<Visitor>::type::result_type,
        void>::value,
    typename std::decay<Visitor>::type::result_type>::type
apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    auto wrapper = detail::visitor_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    visitable.accept(wrapper);
    return;
}

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    !std::is_same<
        typename std::decay<Visitor>::type::result_type,
        void>::value,
    typename std::decay<Visitor>::type::result_type>::type
apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    auto wrapper = detail::visitor_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    
    using cast_t = typename std::conditional<
        std::is_move_constructible<typename Visitor::result_type>::value,
        typename Visitor::result_type&&,
        const typename Visitor::result_type&>::type;
    
    return boost::any_cast<cast_t>(visitable.accept(wrapper));
}

template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    std::is_same<
        typename std::decay<Visitor>::type::result_type,
        void>::value,
    typename std::decay<Visitor>::type::result_type>::type
apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    auto wrapper = detail::visitor_const_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    visitable.accept(wrapper);
    return;
}
    
template<typename Visitor, typename Visitable>
inline typename std::enable_if<
    !std::is_same<
        typename std::decay<Visitor>::type::result_type,
        void>::value,
    typename std::decay<Visitor>::type::result_type>::type
apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    auto wrapper = detail::visitor_const_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    using cast_t = typename std::conditional<
        std::is_move_constructible<typename Visitor::result_type>::value,
        typename Visitor::result_type&&,
        const typename Visitor::result_type&>::type;
    
    return boost::any_cast<cast_t>(visitable.accept(wrapper));
}
    
template<typename Visitor>
struct apply_visitor_delayed
{
    apply_visitor_delayed(Visitor& visitor)
        : visitor(visitor) {}
    
    template<typename Visitable>
    typename Visitor::result_type operator()(Visitable&& visitable) const
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

