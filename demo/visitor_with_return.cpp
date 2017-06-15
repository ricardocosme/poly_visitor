#include "poly_visitor/visitor.hpp"
#include <iostream>

struct Cat;
struct Cockatiel;

using base_visitor = poly_visitor::base_visitor<Cat, Cockatiel>;

struct Animal
{ PURE_VISITABLE(base_visitor) };

struct Cat : Animal
{ VISITABLE(base_visitor) };

struct Cockatiel : Animal
{ VISITABLE(base_visitor) };

struct Speak
{
    using result_type = std::string;

    result_type operator()(const Cat&) const
    { return "Meow..."; }
    result_type operator()(const Cockatiel&) const
    { return "Fiui!"; }
};

int main()
{
    Cockatiel bird;
    const Animal& animal = bird;
    std::cout << poly_visitor::apply_visitor(Speak{}, animal)
              << std::endl;
}
