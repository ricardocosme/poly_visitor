#include "poly_visitor/visitor.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

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
    void operator()(const Cat&) const
    { std::cout << "Meow..." << std::endl; }    
    void operator()(const Cockatiel&) const
    { std::cout << "Fiui!" << std::endl; }
};

int main()
{
    std::vector<std::unique_ptr<Animal>> animals;
    animals.emplace_back(new Cockatiel);
    animals.emplace_back(new Cockatiel);
    animals.emplace_back(new Cat);
    animals.emplace_back(new Cockatiel);
    Speak speak;
    std::for_each(animals.cbegin(), animals.cend(),
                  poly_visitor::apply_visitor(speak));
}
