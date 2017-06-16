# Poly Visitor

Poly Visitor is a C++11 generic component to use the [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern) at
runtime polymorphic solutions. It's a safe, efficient and concise
solution to implement [cyclic visitors](#why-cyclic-visitors-instead-of-acyclic-visitors) in the Object Oriented(OO)
paradigm. 

## Why use Poly Visitor?
In the OO world, adding new polymorphic operations is an intrusive operation and  
all the class hierarchy must be modified and naturally the world must be recompiled. 
In reality, it is a good thing to separate types from operations, the algorithms 
may be free to live independently of the objects. This is an important lesson from Generic Programming(GP) paradigm.
Another question related to the OO world, is that is not uncommon the necessity of a closed type switch to take the concrete type back when all the user have in hands is a base class. It's not easy to write "perfect bases" which
satisfies all the needs without to comeback to the concrete type.

Poly Visitor may help the design of OO based solutions when:
1. The class hierarchy is stable - without addition of new classes frequently;
1. The class hierarchy is composed by a set of know derived classes;
1. New operations are added from time to time;
1. Concrete types must be obtained at some point.

### Why safe?
- The compiler checks if all types of the hierarchy were considered in the visitor implementation. [More about](#what-about-type-switching-with-dynamic_cast)
- Poly Visitor builds up the infra-structure to use the visitor pattern using metaprogramming techniques. [More about](#what-about-handwritten-vistor-pattern)

### Why efficient?
- Poly Visitor only needs one more extra virtual call.
  - **Note**: If the programmer uses the return type of the visitor, it pays for the dynamic allocation of the object under the hood. [More about](#what-is-the-overhead)
  
### Why concise?
- Poly Visitor assumes all the dirty work to leave little work to the programmer. Take a look at the [demo](#demo). [More about](#tell-me-more-why-poly-visitor-is-concise)

## Demo

```c++
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
    using result_type = void;
    
    void operator()(const Cat&) const 
    { std::cout << "Meow..." << std::endl; }    
    void operator()(const Cockatiel&) const
    { std::cout << "Fiui!" << std::endl; }
};

int main()
{
    Cockatiel bird;
    const Animal& animal = bird;
    poly_visitor::apply_visitor(Speak{}, animal);
}
```

## Features
* **Function templates:** the programmer can use function templates in the visitor. It's possible, for an example, to write a generic visitor that takes a forwarding reference:
```c++
struct visitor
{
    using result_type = void;

    template<typename T>
    result_type operator()(T&& o)
    { /* do something */ }
};
```
* **Return of anything copyable:** a visitor can returns anything that is copyable without to change anything in the visitables classes:
```c++
struct visitor
{
    using result_type = std::string;

    template<typename T>
    result_type operator()(T&&)
    { return std::string("something"); }
};
```
* **Delayed visitation:** an easy and concise way to use visitors on sequences of visitables:
```c++
    std::vector<std::unique_ptr<Animal>> animals;
    /* ... */
    Visitor visitor;
    std::for_each(animals.cbegin(), animals.cend(),
                  poly_visitor::apply_visitor(visitor));
```    
* **Constness:** const visitables are supported.

## Depends
Poly Visitor is a header only library.
* Compiler supporting C++11.
* Tested with:
  * Apple LLVM version 8.1.0 (clang-802.0.42) (based on clang ??)
  * GCC 4.8.2
* [Boost.Any](http://www.boost.org/doc/libs/1_64_0/doc/html/any.html)

## Demos and tests
1. Compile with (Boost.Build)[http://www.boost.org/build/]:
```
b2 -sBOOST_PATH=$(BOOST_INSTALL_PATH)
```
2. Run from `stage`, for example:
```
stage/demo_simple
```

## F.A.Q.

#### Why Cyclic Visitors instead of Acyclic Visitors?
Cyclic visitors has one drawback: each class of the hierarchy(visitables) knows the name of all the others. The base class knows the name of all derived classes. It's seems bad but in reality the dependency it's only by the name. Yes, if a new derived class is added, the world is recompiled, but it was already said that it's expected a good level of stability in the hierarchy. The acyclic visitor solve this problem. Nevertheless, there are two advantages in favor of cyclic version:
* The compiler throws an error when a new class is added and some visitor doesn't implement an action to this new type. That it's great. The acyclic version can only do something at runtime;
* The cyclic version is a bit faster because the acyclic version typically needs a `dynamic_cast` to discover the approriate visitor.

#### What about type switching with `dynamic_cast`?
There are better solutions. Type switching using `dynamic_cast` to achieve the concrete type is error prone, unsafe and inefficient. First, it's a handwritten solution.
- A `dynamic_cast` to a base is greed. Any cast after to a concrete type will never be called;
- The compiler does't complain if the programmer forgot to consider all the set of types;
- The world change. The hierarchy may be stable but this isn't mean that it will never be changed. A new derived class probably indicates that the programmer must be consider the new type at all places with type switching. The compiler will not help. Bugs to runtime;
- The `dynamic_cast` is not a free operation and here the programmer needs a bundle of them. Take a look at `test/benchmark_dynamic_cast.cpp`.

#### What about handwritten vistor pattern?
Handwritten solutions of the visitor pattern are error prone and boring. It's necessary a machinery to implement the pattern. A bundle of classes linked to each other through single and multiple inheritance as well as a bit of glue code. Poly Visitor builds up the infra-structure using metaprogramming techniques.

#### What is the overhead?
Poly Visitor only needs one more extra virtual call, but the programmer must be in mind the following when using the return of a visitor:
  * It pays for the dynamic allocation of the returned object under the hood. Poly Visitor uses `boost::any` to make the magic happen.;
  * The object it will be moved to the outside of the visitor if possible(calling the move constructor) otherwise it will be copied.
If this cost are not negligible, the programmer may use a visitor with state and lvalue references to returns things, for example:
```c++
struct visitor
{
    using result_type = std::string;
    
    visitor(std::string& out) : out(out) {}
    std::string& out;

    result_type operator()(const DerivedType&)
    { out = "something"; }

    /* ... another overloads */
};
```

#### Why poly visitor is concise?
The classic C++ runtime polymorphism is an intrusive solution with inheritance and member functions. It's not very easy to put code to use the visitor pattern in the class hierarchy and visitors without messing the things. The mission is more difficult when it is desirable to guarantee features like generic visitors, visitors with return of any type and compile time checking. Poly Visitor demands a minimum code from the user to setup the class hierarchy, to write visitors and visits,


## Acknowledgements
This work is based on the implementation described in *"Modern C++ Design: Generic Programming and Design Patterns Applied"* by Andrei Alexandrescu. The interface and some ideas is based on the implementation of [Boost.Variant](http://www.boost.org/doc/libs/1_64_0/doc/html/variant.html).