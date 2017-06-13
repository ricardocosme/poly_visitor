#pragma once

#include "poly_visitor/type_list.hpp"
#include "poly_visitor/detail/base_visitor.hpp"
#include "poly_visitor/detail/visitor_wrapper.hpp"

namespace poly_visitor {
    
template<typename Types>
struct base_visitor : detail::base_visitor<Types::size, Types>
{
    using types = Types;
};

template<typename Visitor, typename Visitable>
inline
void apply_visitor(Visitor&& visitor, Visitable& visitable)
{
    auto wrapper = detail::visitor_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    visitable.accept(wrapper);
}

template<typename Visitor, typename Visitable>
inline
void apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    auto wrapper = detail::visitor_const_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    visitable.accept(wrapper);
}

template<typename Visitor>
struct apply_visitor_delayed
{
    apply_visitor_delayed(Visitor& visitor)
        : visitor(visitor) {}
    
    template<typename Visitable>
    void operator()(Visitable&& visitable) const
    {
        return apply_visitor(visitor, visitable);
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
virtual void accept(BASE_VISITOR&) = 0;\
virtual void accept(BASE_VISITOR&) const = 0;

#define VISITABLE(BASE_VISITOR)\
using base_visitor__ = BASE_VISITOR;\
virtual void accept(BASE_VISITOR& visitor)\
{return visitor.visit(*this);}\
virtual void accept(BASE_VISITOR& visitor) const\
{return visitor.visit(*this);}

