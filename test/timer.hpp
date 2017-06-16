#pragma once

#include <chrono>
#include <iostream>

template<typename Duration>
struct DurationTraits
{ static constexpr const char* unit = ""; };
        
template<>
struct DurationTraits<std::chrono::seconds>
{ static constexpr const char* unit = "s"; };
        
template<>
struct DurationTraits<std::chrono::milliseconds>
{ static constexpr const char* unit = "ms"; };
        
template<>
struct DurationTraits<std::chrono::microseconds>
{ static constexpr const char* unit = "us"; };
        
template<>
struct DurationTraits<std::chrono::nanoseconds>
{ static constexpr const char* unit = "ns"; };
        
template<typename Duration>        
struct timer
{
    timer()
    {
        t0 = std::chrono::high_resolution_clock::now();
    }
    timer(std::string plabel)
        : timer()
    {
        label = std::move(plabel);
    }
    ~timer()
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<Duration>(t1-t0);
        std::cout << label << " took "
                  << duration.count()
                  << DurationTraits<Duration>::unit
                  << std::endl;
    }
    std::string label{"timer"};
    std::chrono::time_point<std::chrono::high_resolution_clock> t0;
};

