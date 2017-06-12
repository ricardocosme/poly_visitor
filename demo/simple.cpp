#include "poly_visitor/visitor.hpp"
#include <iostream>

struct Cat;
struct Cockatiel;

using base_visitor = poly_visitor::base_visitor<
    poly_visitor::TypeList<Cat, Cockatiel>>;

struct Animal
{ PURE_VISITABLE(base_visitor); };

struct Cat : Animal
{ VISITABLE(base_visitor); };

struct Cockatiel : Animal
{ VISITABLE(base_visitor); };

struct Speak
{
    void operator()(const Cat&) 
    { std::cout << "Meow..." << std::endl; }    
    void operator()(const Cockatiel&) 
    { std::cout << "Fiui!" << std::endl; }
};

int main()
{
    Cockatiel bird;
    const Animal& animal = bird;
    poly_visitor::apply_visitor(Speak{}, animal);
}
