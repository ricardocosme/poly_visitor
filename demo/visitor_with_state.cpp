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
    Speak(std::string& out) : out(out) {}
    
    void operator()(const Cat&) 
    { out = "Meow..."; }    
    void operator()(const Cockatiel&) 
    { out = "Fiui!"; }

    std::string& out;
};

int main()
{
    Cockatiel bird;
    const Animal& animal = bird;
    std::string out;
    Speak speak{out};
    poly_visitor::apply_visitor(speak, animal);
    std::cout << out << std::endl;
}
