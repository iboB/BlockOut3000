// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once
#include "lib/vec3.hpp"

inline ivec3 RotateX_CW(ivec3 v, int grid)
{
    return {v.x, v.z, grid - v.y - 1};
}

inline ivec3 RotateX_CCW(ivec3 v, int grid)
{
    return {v.x, grid - v.z - 1, v.y};
}

inline ivec3 RotateY_CW(ivec3 v, int grid)
{
    return {v.z, v.y, grid - v.x - 1};
}

inline ivec3 RotateY_CCW(ivec3 v, int grid)
{
    return {grid - v.z - 1, v.y, v.x};
}

inline ivec3 RotateZ_CW(ivec3 v, int grid)
{
    return {v.y, grid - v.x - 1, v.z};
}

inline ivec3 RotateZ_CCW(ivec3 v, int grid)
{
    return {grid - v.y - 1, v.x, v.z};
}
