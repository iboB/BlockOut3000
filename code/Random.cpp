// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Random.hpp"

#include <random>

namespace
{

// simplified random number generator class (based on a template parameter an actual random number generator)
// used where the need for a random number is in a basic case, to avoid writing lots of code
// for the most common distributions
template <typename Rng>
class simplified_rng
{
public:

    uint32_t rndu32()
    {
        return m_uint32Dist(rng);
    }

    uint32_t rndu32(uint32_t a, uint32_t b)
    {
        std::uniform_int_distribution<uint32_t> dist(a, b);
        return dist(rng);
    }

    float rndf()
    {
        return m_floatDist(rng);
    }

    Rng rng;

private:
    std::uniform_int_distribution<uint32_t> m_uint32Dist;
    std::uniform_real_distribution<float> m_floatDist;
};

simplified_rng<std::mt19937> rnd;
// using mersenne twister since it's the only one that seems to produce the same values on all platforms

}

void Random::seed(uint32_t seed)
{
    rnd.rng.seed(seed);
}

uint32_t Random::u32()
{
    return rnd.rndu32();
}
