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
    Speak(std::string& out) : out(out) {}
    std::string& out;
    
    void operator()(Cat&) 
    { out = "cat";}
    void operator()(Cockatiel&) 
    { out = "cockatiel";}
    void operator()(const Cat&) 
    { out = "const cat";}
    void operator()(const Cockatiel&) 
    { out = "const cockatiel";}
};

int main()
{
    std::string out;
    Cockatiel bird;
    {
        Animal& animal = bird;
        Speak visitor{out};
        poly_visitor::apply_visitor(visitor, animal);
        if(out != "cockatiel") abort();
    }
    {
        const Animal& animal = Cat{};
        Speak visitor{out};
        poly_visitor::apply_visitor(visitor, animal);
        if(out != "const cat") abort();
    }
}
