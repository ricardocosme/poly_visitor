#pragma once

#include <utility>

namespace poly_visitor { namespace detail {

template<typename... Lambdas>    
struct match_visitor;
    
template<typename Lambda>    
struct match_visitor<Lambda> : Lambda
{
    using Lambda::operator();

    template<typename L>
    match_visitor(L&& lambda) : Lambda(std::forward<L>(lambda))
    {}
};

template<typename Lambda, typename... Lambdas>
struct match_visitor<Lambda, Lambdas...> : Lambda, match_visitor<Lambdas...>
{
    using Lambda::operator();
    using match_visitor<Lambdas...>::operator();
    
    template<typename L, typename... Ls>
    match_visitor(L&& lambda, Ls&&... lambdas)
        : Lambda(std::forward<L>(lambda))
        , match_visitor<Lambdas...>(std::forward<Ls>(lambdas)...)
    {}
};

        
}}
