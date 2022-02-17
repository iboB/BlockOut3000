// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "sokol-gfx.h"

struct Mesh
{
    Mesh() = default;
    Mesh(sg_buffer buf, int num)
        : vb(buf)
        , numElements(num)
    {}
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other)
        : vb(other.vb)
        , numElements(other.numElements)
    {
        other.vb.id = SG_INVALID_ID;
        other.numElements = 0;
    }
    Mesh& operator=(Mesh&& other);
    ~Mesh();

    sg_buffer vb = {SG_INVALID_ID};
    int numElements = 0;

    template <typename T, int N>
    static Mesh create(const T (&data)[N])
    {
        return create(data, sizeof(T) * N, N);
    }

    template <typename Container>
    static Mesh create(const Container& c)
    {
        return create(c.data(), c.size() * sizeof(typename Container::value_type), int(c.size()));
    }

    void destroy();

    bool valid() const { return vb.id == SG_INVALID_ID; }

private:
    static Mesh create(const void* buf, size_t bufSize, int numElements);    
};
