#include "poly_visitor/visitor.hpp"
#include <iostream>

struct bird;
struct cockatiel;
struct eagle;

using base_visitor = poly_visitor::base_visitor
    <bird, cockatiel, eagle>;

struct animal
{ PURE_VISITABLE(base_visitor) };

struct bird : animal
{ VISITABLE(base_visitor) };

struct cockatiel : bird
{ VISITABLE(base_visitor) };

struct eagle : bird
{ VISITABLE(base_visitor) };

struct visitor
{
    using result_type = void;
    
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
