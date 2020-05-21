#include "hat_color.h"
#include <random>
#include <iostream>
#include <future>
#include <thread>


uint8_t const NUM_HATS = 100;
unsigned const experiments_per_thread = 100000;

using HatVec = std::vector<HatColour>;

HatVec assign_hats(uint32_t seed)
{
    std::mt19937 gen(seed);
    HatDist d;
    HatVec hats(NUM_HATS);
    for(unsigned i=0; i < NUM_HATS; ++i)
    {
        hats[i] = d(gen);
    }
    return hats;
}

HatVec calc_blue_parity(HatVec const& hats)
{
    HatVec blue_parity(NUM_HATS);
    HatColour parity = HatColour::red;
    for(unsigned i=0; i < NUM_HATS; ++i)
    {
        blue_parity[i] = parity;
        parity = (blue_parity[i] ^ hats[i]);
    }
    return blue_parity;
}

uint8_t experiment(uint32_t seed)
{
    HatVec hats = assign_hats(seed);

    HatVec blue_parity = calc_blue_parity(hats);  // parity of blue hats user i can see.

    uint8_t kills = 0;

    // guy at end starts with red xored with the blue he can see
    HatColour par = HatColour::red;

    for(int i=NUM_HATS-1; i >= 0; --i)
    {
        HatColour choice = blue_parity[i] ^ par;
        if(choice != hats[i])
        {
            ++kills;
        }
        par = par ^ choice;
    }

    return kills;
}

unsigned experiments(uint32_t seed)
{
    unsigned kills = 0;
    for(unsigned i=0; i < experiments_per_thread; ++i)
    {
        kills += experiment(seed);
        ++seed;
    }
    return kills;
}

int main(int argc, const char** argv)
{
    std::random_device rd;

    std::vector<std::future<unsigned> > results;

    for(unsigned i=0; i < 32; ++i)
    {
       results.emplace_back(std::async(std::launch::async, experiments, rd()));
    }
    unsigned num_experiments = results.size() * experiments_per_thread;

    unsigned total_kills = 0;
    for(auto& e: results)
    {
        total_kills += e.get();
    }
    std::cout << "Found " << total_kills << " total kills over " << num_experiments << " experiments, an average of " << (float(total_kills) / float(num_experiments)) << " deaths per experiment." << std::endl;

    return 0;
}

