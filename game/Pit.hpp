// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/Mesh.hpp"
#include "lib/matrix.hpp"
#include "lib/vec3.hpp"

class Renderer;

class Pit
{
public:
    Pit(ivec3 size);
    ~Pit();

    void draw(Renderer& r);

    const matrix& projView() const { return m_projView; }

private:
    // logical data
    ivec3 m_size;

    // physical data
    Mesh m_solid;
    Mesh m_wire;
    void createBuffers();

    // we want the camera to look at 000
    // since the pit takes dimensions from 000 to whd
    // we also use this matrix to transform it to the appropriate place
    matrix m_projView;
};