// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// a tetris figure template. Has no actual position or pit info

#include "Primitives.hpp"

#include <Magnum/GL/Mesh.h>

#include <string>
#include <vector>

class FigureTemplate
{
public:
    FigureTemplate();
    ~FigureTemplate();

    void preparePhysicalData();

    void draw(const Magnum::Color4& solidColor, const Magnum::Color4& wireColor) const;

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    const Magnum::Vector3& rotationCenter() const { return m_rotationCenter; }
    void setRotationCenter(const Magnum::Vector3& c) { m_rotationCenter = c; }

    void addElement(const Magnum::Vector3i& elem) { m_elements.push_back(elem); }

    const std::vector<Magnum::Vector3i>& elements() const { return m_elements; }
private:

    //////////////////////////////////////
    // logical data
    std::string m_name;
    std::vector<Magnum::Vector3i> m_elements;

    Magnum::Vector3 m_rotationCenter;

    //////////////////////////////////////
    // rendering (physical data)
    bool m_hasPhysicalData = false;
    mutable Magnum::GL::Mesh m_wire;
    mutable Magnum::GL::Mesh m_solid;
};
