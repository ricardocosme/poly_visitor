#include "poly_visitor/visitor.hpp"
#include "timer.hpp"
#include <iostream>
#include <string>

struct D1;
struct D2;
struct D3;
struct D4;
struct D5;
struct D6;
struct D7;

using base_visitor = poly_visitor::base_visitor
    <D1, D2, D3, D4, D5, D6, D7>;

struct B
{ POLY_VISITOR_PURE_VISITABLE(base_visitor) };
struct D1 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D1() = default;
};
struct D2 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D2() = default;
};
struct D3 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D3() = default;
};
struct D4 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D4() = default;
};
struct D5 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D5() = default;
};
struct D6 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D6() = default;
};
struct D7 : B
{
    POLY_VISITOR_VISITABLE(base_visitor)
    virtual ~D7() = default;
};

struct Visitor
{
    void operator()(const D1&) const
    {}
    void operator()(const D2&) const
    {}
    void operator()(const D3&) const
    {}
    void operator()(const D4&) const
    {}
    void operator()(const D5&) const
    {}
    void operator()(const D6&) const
    {std::cout << "d6"<< std::endl;}
    void operator()(const D7&) const
    {}
};

std::size_t n(1);

int main(int argc, char **argv)
{
    if(argc > 1)
        n = std::stol(argv[1]);
    
    std::cout << "usage: benchmark [number_of_iterations]"
              << std::endl << std::endl
              << "[Number of iterations] = " << n
              << std::endl;
    
    D7 o;
    B& base = o;
    {
        timer<std::chrono::microseconds> t("dynamic_cast");
        for(std::size_t i(0); i < n; ++i)
        {
            if(auto p = dynamic_cast<D1*>(&base))
            {}
            else if(auto p = dynamic_cast<D2*>(&base))
            {}
            else if(auto p = dynamic_cast<D3*>(&base))
            {}
            else if(auto p = dynamic_cast<D4*>(&base))
            {}
            else if(auto p = dynamic_cast<D5*>(&base))
            {}
            else if(auto p = dynamic_cast<D6*>(&base))
            {std::cout << "d6"<< std::endl;}
            else if(auto p = dynamic_cast<D7*>(&base))
            {}
        }
    }
    {
        timer<std::chrono::microseconds> t("poly visitor");
        for(std::size_t i(0); i < n; ++i)
            poly_visitor::apply_visitor(Visitor{}, base);
    }
}
