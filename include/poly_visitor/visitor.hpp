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
    auto wrapper = detail::visitor_visitable_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    visitable.accept(wrapper);
}

template<typename Visitor, typename Visitable>
inline
void apply_visitor(Visitor&& visitor, const Visitable& visitable)
{
    auto wrapper = detail::visitor_const_visitable_wrapper<
        Visitor,
        typename Visitable::base_visitor__>(visitor);
    visitable.accept(wrapper);
}
    
}


#define PURE_VISITABLE(Visitor)\
using base_visitor__ = Visitor;\
virtual void accept(Visitor&)  = 0;\
virtual void accept(Visitor&) const = 0;

#define VISITABLE(Visitor)\
using base_visitor__ = Visitor;\
virtual void accept(Visitor& visitor)\
{return visitor.visit(*this);}\
virtual void accept(Visitor& visitor) const\
{return visitor.visit(*this);}

