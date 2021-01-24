// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Primitives.hpp"
#include <vector>

class Cube
{
public:
    static Cube& instance();

    const std::vector<LineSegment>& wireSegments() const { return m_wireSegments; }
    const std::vector<LineSegment>& wideWireSegments() const { return m_wideWireSegments; }
    const std::vector<Triangle>& triangles() const { return m_triangles; }

private:
    Cube();
    ~Cube();

    std::vector<LineSegment> m_wireSegments;

    // a helper array of segments which are a tiny bit bigger in order to draw them
    // with depth testing enabled while they still remain visible
    std::vector<LineSegment> m_wideWireSegments;


    std::vector<Triangle> m_triangles;
};
