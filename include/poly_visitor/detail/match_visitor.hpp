#pragma once

namespace poly_visitor { namespace detail {

template<typename... Lambdas>    
struct match_visitor;
    
template<typename Lambda>    
struct match_visitor<Lambda> : Lambda
{
    using Lambda::operator();
    match_visitor(Lambda lambda) : Lambda(lambda)
    {}
};

template<typename Lambda, typename... Lambdas>
struct match_visitor<Lambda, Lambdas...> : Lambda, match_visitor<Lambdas...>
{
    using Lambda::operator();
    using match_visitor<Lambdas...>::operator();
    match_visitor(Lambda lambda, Lambdas... lambdas)
        : Lambda(lambda)
        , match_visitor<Lambdas...>(lambdas...)
    {}
};

        
}}
