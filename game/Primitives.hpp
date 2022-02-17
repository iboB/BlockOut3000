// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/vec3.hpp"

#include <yama/util.hpp>

#include <array>
#include <algorithm>

// defines geometric primitives

template <size_t N>
using Primitive = std::array<vec3, N>;

using LineSegment = Primitive<2>;
using Triangle = Primitive<3>;

// A class that makes primitives avaialbale as keys for sets of maps
// Warning! Using this class may be slow
// In order to compensate for possible primitive winding this class sorts the input primitives
// It doesn't change them, but it copies them to do the sorting.
struct PrimitiveStrictWeakOrdering
{
    template <size_t N>
    bool operator()(const Primitive<N>& a, const Primitive<N>& b) const
    {
        // cannot just compare triangles a-a b-b c-c, because trignales with different windings must match
        yama::strict_weak_ordering o;

        auto ca = a;
        std::sort(ca.begin(), ca.end(), o);
        auto cb = b;
        std::sort(cb.begin(), cb.end(), o);

        for(size_t i = 0; i < N; ++i)
        {
            if(o(ca.at(i), cb.at(i)))
                return true;
            else if(o(cb.at(i), ca.at(i)))
                return false;
        }

        return false;
    }
};
