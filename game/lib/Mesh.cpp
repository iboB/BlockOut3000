// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Mesh.hpp"

Mesh::~Mesh()
{
    destroy();
}

Mesh& Mesh::operator=(Mesh&& other)
{
    destroy();
    vb = other.vb;
    numElements = other.numElements;
    other.vb.id = SG_INVALID_ID;
    other.numElements = 0;
    return *this;
}

void Mesh::destroy()
{
    if (valid()) sg_destroy_buffer(vb);
    numElements = 0;
}

Mesh Mesh::create(const void* buf, size_t bufSize, int numElements)
{
    sg_buffer_desc desc = {};
    desc.data.ptr = buf;
    desc.data.size = bufSize;
    return Mesh(sg_make_buffer(desc), numElements);
}