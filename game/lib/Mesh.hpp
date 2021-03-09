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
    Mesh& operator=(Mesh&& other)
    {
        vb = other.vb;
        numElements = other.numElements;
        other.vb.id = SG_INVALID_ID;
        other.numElements = 0;
        return *this;
    }
    ~Mesh();

    sg_buffer vb = {SG_INVALID_ID};
    int numElements = 0;

    template <typename T, int N>
    static Mesh create(const T (&data)[N])
    {
        sg_buffer_desc desc = {};
        desc.data.ptr = data;
        desc.data.size = sizeof(T) * N;
        return Mesh(sg_make_buffer(desc), N);
    }

    template <typename Container>
    static Mesh create(const Container& c)
    {
        sg_buffer_desc desc = {};
        desc.data.ptr = c.data();
        desc.data.size = c.size() * sizeof(typename Container::value_type);
        return Mesh(sg_make_buffer(desc), int(c.size()));
    }
};
