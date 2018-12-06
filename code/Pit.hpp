// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Magnum.h>
#include <vector>

class PitLayer;

// Game level class

class Pit
{
public:
    Pit(const Magnum::Vector3ui& size);
    ~Pit();

    void update(uint32_t dt);

    void draw();

    const Magnum::Vector3ui& size() const { return m_size; }

    bool canFitFigure(const std::vector<Magnum::Vector3i>& elements) const;

    void adoptFigure(const std::vector<Magnum::Vector3i>& elements);

    static const int MAX_SPEED = 10;
    int speed() const { return m_speed; }

    uint32_t topNonEmptyLayer() const { return m_topNonEmptyLayer; }

private:
    // logical data
    const Magnum::Vector3ui m_size;

    //unsigned indexInData(unsigned layer, unsigned row, unsigned col) const { return layer * m_size.x * m_size.y + row * m_size.x + col; }
    std::vector<PitLayer*> m_layers;
    uint32_t m_topNonEmptyLayer = 0;

    int m_speed = 0;

    // physical data
    // box
    Magnum::GL::Mesh m_wire;
    Magnum::GL::Mesh m_solid;

    void createBuffers();

    // we wat the camera to look at 000
    // since the pit takes dimensions from 000 to whd
    // we also use this matrix to transform it to the appropriate place
    Magnum::Matrix4 m_projView;
};
