#include "poly_visitor.hpp"
#include <iostream>

struct Cat;
struct Cockatiel;

using base_visitor = poly_visitor::base_visitor<Cat, Cockatiel>;

struct Animal
{ POLY_VISITOR_PURE_VISITABLE(base_visitor) };

struct Cat : Animal
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct Cockatiel : Animal
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct Speak
{
    void operator()(Cat&) const 
    { std::cout << "Meow..." << std::endl; }    
    void operator()(Cockatiel&) const
    { std::cout << "Fiui!" << std::endl; }
};

int main()
{
    Cockatiel bird;
    Animal& animal = bird;
    poly_visitor::apply_visitor(Speak{}, animal);
}
