#include "poly_visitor.hpp"
#include <iostream>

#if __cplusplus >= 201402L
#define HAS_CPP14_SUPPORT
#endif

struct Cat;
struct Cockatiel;

using base_visitor = poly_visitor::base_visitor<Cat, Cockatiel>;

struct Animal
{ POLY_VISITOR_PURE_VISITABLE(base_visitor) };

struct Cat : Animal
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct Cockatiel : Animal
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct movable
{
    movable() = default;
    movable(movable&&)
    { std::cout << "move ctor" << std::endl; }
};

struct movable_copyable
{
    movable_copyable() = default;
    movable_copyable(const movable_copyable&)
    { std::cout << "copy ctor" << std::endl; }
    movable_copyable(movable_copyable&&)
    { std::cout << "move ctor" << std::endl; }
};

int main()
{
    Cockatiel bird;
#ifdef HAS_CPP14_SUPPORT
    {
        Animal& animal = bird;
        {
            poly_visitor::match(animal, [p = movable{}](auto&&){});
        }
        {
            auto lambda = [p = movable_copyable{}](auto&&){};   
            poly_visitor::match(animal, lambda);
        }
        {
            auto lambda = [p = movable_copyable{}](Cat){};   
            poly_visitor::match(animal,
                                [p = movable{}](Cockatiel){},
                                lambda);
        }
        {
            auto lambda = [p = movable_copyable{}](Cat){};   
            poly_visitor::match(animal,
                                lambda,
                                [p = movable{}](Cockatiel){});
        }
        {
            auto lambda = [p = movable{}](Cat){};   
            poly_visitor::match(animal,
                                std::move(lambda),
                                [p = movable{}](Cockatiel){});
        }
        {
            auto lambda1 = [p = movable_copyable{}](Cat){};   
            auto lambda2 = [p = movable_copyable{}](Cockatiel){};   
            poly_visitor::match(animal, lambda1, lambda2);
        }
    }
    {
        const Animal& animal = bird;
        {
            poly_visitor::match(animal, [p = movable{}](auto&&){});
        }
        {
            auto lambda = [p = movable_copyable{}](auto&&){};   
            poly_visitor::match(animal, lambda);
        }
        {
            auto lambda = [p = movable_copyable{}](const Cat&){};   
            poly_visitor::match(animal,
                                [p = movable{}](const Cockatiel&){},
                                lambda);
        }
        {
            auto lambda = [p = movable_copyable{}](const Cat&){};   
            poly_visitor::match(animal,
                                lambda,
                                [p = movable{}](const Cockatiel&){});
        }
        {
            auto lambda = [p = movable{}](const Cat&){};   
            poly_visitor::match(animal,
                                std::move(lambda),
                                [p = movable{}](const Cockatiel&){});
        }
        {
            auto lambda1 = [p = movable_copyable{}](const Cat&){};   
            auto lambda2 = [p = movable_copyable{}](const Cockatiel&){};   
            poly_visitor::match(animal, lambda1, lambda2);
        }
    }
#endif
}
