#pragma once

#include "poly_visitor/detail/visitor.hpp"
#include "poly_visitor/detail/visitor_const.hpp"

namespace poly_visitor { namespace detail {

template<typename VisitorWrapper,
         typename BaseVisitor>
struct visitor_wrapper_base : visitor<
    BaseVisitor::types::size,
    typename BaseVisitor::types, VisitorWrapper, BaseVisitor>
{};

template<typename Visitor, typename BaseVisitor>
struct visitor_wrapper : visitor_wrapper_base<
    visitor_wrapper<Visitor, BaseVisitor>,
    BaseVisitor>
{
    visitor_wrapper(Visitor& uvisitor)
        : uvisitor(uvisitor)
    {}
    
    template<typename T>
    void visit(T&& o)
    {
        uvisitor(std::forward<T>(o));
    }
    
    Visitor& uvisitor;
};
 
template<typename VisitorWrapper, typename BaseVisitor>
struct visitor_const_wrapper_base : visitor_const<
    BaseVisitor::types::size,
    typename BaseVisitor::types, VisitorWrapper, BaseVisitor>
{};
        
template<typename Visitor, typename BaseVisitor>
struct visitor_const_wrapper : visitor_const_wrapper_base<
    visitor_const_wrapper<Visitor, BaseVisitor>,
    BaseVisitor>
{
    visitor_const_wrapper(Visitor& uvisitor)
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
