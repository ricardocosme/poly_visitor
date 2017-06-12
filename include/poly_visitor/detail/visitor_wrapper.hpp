#pragma once

#include "poly_visitor/detail/visitor_visitable.hpp"
#include "poly_visitor/detail/visitor_const_visitable.hpp"

namespace poly_visitor { namespace detail {

template<typename DV, typename UserBaseVisitor>
struct visitor_visitable_wrapper_base : visitor_visitable<
    UserBaseVisitor::types::size,
    typename UserBaseVisitor::types, DV, UserBaseVisitor>
{};

template<typename Visitor, typename UserBaseVisitor>
struct visitor_visitable_wrapper : visitor_visitable_wrapper_base<
    visitor_visitable_wrapper<Visitor, UserBaseVisitor>,
    UserBaseVisitor>
{
    visitor_visitable_wrapper(Visitor& uvisitor)
        : uvisitor(uvisitor)
    {}
    
    template<typename T>
    void visit(T&& o)
    {
        uvisitor(std::forward<T>(o));
    }
    
    Visitor& uvisitor;
};
 
template<typename DV, typename UserBaseVisitor>
struct visitor_const_visitable_wrapper_base : visitor_const_visitable<
    UserBaseVisitor::types::size,
    typename UserBaseVisitor::types, DV, UserBaseVisitor>
{};
        
template<typename Visitor, typename UserBaseVisitor>
struct visitor_const_visitable_wrapper : visitor_const_visitable_wrapper_base<
    visitor_const_visitable_wrapper<Visitor, UserBaseVisitor>,
    UserBaseVisitor>
{
    visitor_const_visitable_wrapper(Visitor& uvisitor)
        : uvisitor(uvisitor)
    {}
    
    template<typename T>
    void visit(T&& o)
    {
        uvisitor(std::forward<T>(o));
    }
    Visitor& uvisitor;
};
        
}}
