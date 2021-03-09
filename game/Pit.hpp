#pragma once

#include "lib/vec3.hpp"
#include "lib/matrix.hpp"
#include "lib/Mesh.hpp"

class Renderer;

class Pit
{
public:
    Pit(ivec3 size);
    ~Pit();

    void draw(Renderer& r);

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