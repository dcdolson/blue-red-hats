#pragma once
#include <random>
#include <iostream>

enum class HatColour: uint8_t { red, blue };

HatColour operator^(HatColour a, HatColour b)
{
    return (a == b)? HatColour::red: HatColour::blue;
}

std::ostream& operator<<(std::ostream& s, HatColour c)
{
    switch(c)
    {
        case HatColour::red:
            s << "red";
            break;
        case HatColour::blue:
            s << "blue";
            break;
    }
    return s;
}

// random distribution of hats
class HatDist
{
private:
    using t_Dist = std::uniform_int_distribution<unsigned int>;
    t_Dist m_dist;

public:
    HatDist(): m_dist(0,1) {}

    template<typename GEN>
    HatColour operator()(GEN& gen)
    {
        unsigned i = m_dist(gen);
        return i==0? HatColour::blue: HatColour::red;
    }
};
