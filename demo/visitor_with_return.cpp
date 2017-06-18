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
    std::string operator()(const Cat&) const
    { return "Meow..."; }
    std::string operator()(const Cockatiel&) const
    { return "Fiui!"; }
};

int main()
{
    Cockatiel bird;
    const Animal& animal = bird;
    std::cout << poly_visitor::apply_visitor(Speak{}, animal)
              << std::endl;
}
