#include "poly_visitor/visitor.hpp"
#include <iostream>

struct bird;
struct cockatiel;
struct eagle;

using base_visitor = poly_visitor::base_visitor
    <bird, cockatiel, eagle>;

struct animal
{ POLY_VISITOR_PURE_VISITABLE(base_visitor) };

struct bird : animal
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct cockatiel : bird
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct eagle : bird
{ POLY_VISITOR_VISITABLE(base_visitor) };

struct visitor
{
    void operator()(const bird&) 
    { std::cout << "bird" << std::endl; }
    void operator()(const cockatiel&) 
    { std::cout << "cockatiel" << std::endl; }
    void operator()(const eagle&) 
    { std::cout << "eagle" << std::endl; }
};

int main()
{
    cockatiel o;
    bird& i = o;
    poly_visitor::apply_visitor(visitor{}, i);
}
