#pragma once
#include "sokol-gfx.h"

template <typename T>
sg_range sg_make_range(const T& t)
{
    return sg_range{&t, sizeof(T)};
}
