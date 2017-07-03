#pragma once

#include "poly_visitor/detail/visitor.hpp"
#include "poly_visitor/detail/visitor_const.hpp"

#include <boost/any.hpp>

#include <utility>

namespace poly_visitor { namespace detail {

template<typename VisitorWrapper,
         typename BaseVisitor>
struct visitor_wrapper_base : visitor_hierarchy<
    typename BaseVisitor::visitables, VisitorWrapper, BaseVisitor>
{};

template<typename Visitor, typename BaseVisitor, typename ResultType>
struct visitor_wrapper : visitor_wrapper_base<
    visitor_wrapper<Visitor, BaseVisitor, ResultType>,
    BaseVisitor>
{
    visitor_wrapper(Visitor& uvisitor)
        : uvisitor(uvisitor)
    {}
    
    template<typename T>
    boost::any ctor_any(T&& o, std::true_type)
    {
        return uvisitor(std::forward<T>(o));
    }
    
    template<typename T>
    boost::any ctor_any(T&& o, std::false_type)
    {
        const auto& ans = uvisitor(std::forward<T>(o));
        return boost::any(ans);
    }
    
    template<typename T>
    boost::any visit(T&& o, std::true_type)
    {
        uvisitor(std::forward<T>(o));
        return boost::any{};
    }

    template<typename T>
    boost::any visit(T&& o, std::false_type)
    {
        return ctor_any(std::forward<T>(o),
                        typename std::is_move_constructible<
                        ResultType>::type());
    }
    
    template<typename T>
    boost::any visit(T&& o)
    {
        return visit(std::forward<T>(o),
                     typename std::is_same<ResultType, void>::type{});
    }
    Visitor& uvisitor;
};
 
template<typename VisitorWrapper, typename BaseVisitor>
struct visitor_const_wrapper_base : visitor_const_hierarchy<
    typename BaseVisitor::visitables, VisitorWrapper, BaseVisitor>
{};
        
template<typename Visitor, typename BaseVisitor, typename ResultType>
struct visitor_const_wrapper : visitor_const_wrapper_base<
    visitor_const_wrapper<Visitor, BaseVisitor, ResultType>,
    BaseVisitor>
{
    visitor_const_wrapper(Visitor& uvisitor)
        : uvisitor(uvisitor)
    {}

    template<typename T>
    boost::any ctor_any(T&& o, std::true_type)
    {
        return uvisitor(std::forward<T>(o));
    }
    
    template<typename T>
    boost::any ctor_any(T&& o, std::false_type)
    {
        const auto& ans = uvisitor(std::forward<T>(o));
        return boost::any(ans);
    }
    
    template<typename T>
    boost::any visit(T&& o, std::true_type)
    {
        uvisitor(std::forward<T>(o));
        return boost::any{};
    }
    
    template<typename T>
    boost::any visit(T&& o, std::false_type)
    {
        return ctor_any(std::forward<T>(o),
                        typename std::is_move_constructible<
                        ResultType>::type());
    }
    
    template<typename T>
    boost::any visit(T&& o)
    {
        return visit(std::forward<T>(o),
                     typename std::is_same<ResultType, void>::type{});
    }
    Visitor& uvisitor;
};
        
}}
