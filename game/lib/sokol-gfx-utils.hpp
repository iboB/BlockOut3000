// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once
#include "sokol-gfx.h"

template <typename T>
sg_range sg_make_range(const T& t)
{
    return sg_range{&t, sizeof(T)};
}
