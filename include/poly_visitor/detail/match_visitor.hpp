#pragma once

#include <utility>

namespace poly_visitor { namespace detail {

template<typename... Lambdas>    
struct match_visitor;
    
template<typename Lambda>    
struct match_visitor<Lambda> : Lambda
{
    using Lambda::operator();
    match_visitor(Lambda lambda) : Lambda(std::move(lambda))
    {}
};

template<typename Lambda, typename... Lambdas>
struct match_visitor<Lambda, Lambdas...> : Lambda, match_visitor<Lambdas...>
{
    using Lambda::operator();
    using match_visitor<Lambdas...>::operator();
    match_visitor(Lambda lambda, Lambdas... lambdas)
        : Lambda(std::move(lambda))
        , match_visitor<Lambdas...>(std::move(lambdas)...)
    {}
};

        
}}
