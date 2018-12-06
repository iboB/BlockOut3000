// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <cstdint>

// random generator for a single game
// seeded at the start of each one
// (could potentially be used for replaying a game with the same figure order)
struct Random
{
    static void seed(uint32_t seed);
    static uint32_t u32();
};
