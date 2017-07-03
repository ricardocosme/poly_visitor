#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>

namespace poly_visitor { namespace detail {

template<typename Visitor, typename Visitalbe, typename = void>
struct has_visit : std::false_type {};
        
template<typename Visitor, typename Visitable>
struct has_visit<Visitor, Visitable,
                 decltype((void)std::declval<Visitor>()
                          .operator()(std::declval<Visitable&>()))> 
    : std::true_type
{};

template<typename Visitor, typename Visitable>        
struct has_visit_assert
{
    static_assert(has_visit<Visitor, Visitable>::value,
                  "The visitor doesn't implement a visit function to a "\
                  "specific visitable. Please, check the error message "\
                  "about the instantiation of template class "\
                  "'poly_visitor::detail::has_visit_assert<Visitor, Visitable>' "\
                  "to figure out the type of Visitable. ");
    using type = std::size_t;
};
                
}}
