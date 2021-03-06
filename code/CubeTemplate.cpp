// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "CubeTemplate.hpp"

using namespace Magnum;

static Vector3 v(float x, float y, float z)
{
    return { x, y, z };
}

CubeTemplate::CubeTemplate()
{
    m_wireSegments = {
        // bottom
        { v(0, 0, 0), v(1, 0, 0) },
        { v(0, 1, 0), v(1, 1, 0) },
        { v(0, 0, 0), v(0, 1, 0) },
        { v(1, 0, 0), v(1, 1, 0) },

        // tp
        { v(0, 0, 1), v(1, 0, 1) },
        { v(0, 1, 1), v(1, 1, 1) },
        { v(0, 0, 1), v(0, 1, 1) },
        { v(1, 0, 1), v(1, 1, 1) },

        // sides
        { v(0, 0, 0), v(0, 0, 1) },
        { v(1, 0, 0), v(1, 0, 1) },
        { v(1, 1, 0), v(1, 1, 1) },
        { v(0, 1, 0), v(0, 1, 1) },
    };

    m_wideWireSegments = m_wireSegments;

    for (auto& w : m_wideWireSegments)
    {
        for (auto& elem : w)
        {
            static const float e = 0.005f;
            //for (auto& c : elem)
            //{
            for (int i = 0; i < 3; ++i)
            {
                auto& c = elem[i];
                if (c == 0)
                {
                    c -= e;
                }
                else
                {
                    c += e;
                }
            }
        }
    }

    m_triangles =
    {
        //bottom
        {v(0, 0, 0), v(1, 1, 0), v(1, 0, 0)},
        {v(1, 1, 0), v(0, 0, 0), v(0, 1, 0)},

        // top
        {v(0, 0, 1), v(1, 0, 1), v(1, 1, 1)},
        {v(1, 1, 1), v(0, 1, 1), v(0, 0, 1)},

        //south
        {v(0, 0, 0), v(1, 0, 0), v(0, 0, 1)},
        {v(1, 0, 0), v(1, 0, 1), v(0, 0, 1)},

        // north
        {v(0, 1, 0), v(0, 1, 1), v(1, 1, 0)},
        {v(1, 1, 0), v(0, 1, 1), v(1, 1, 1)},

        //east
        {v(1, 0, 0), v(1, 1, 1), v(1, 0, 1)},
        {v(1, 1, 1), v(1, 0, 0), v(1, 1, 0)},

        // west
        {v(0, 0, 0), v(0, 0, 1), v(0, 1, 1)},
        {v(0, 1, 1), v(0, 1, 0), v(0, 0, 0)},
    };
}

CubeTemplate::~CubeTemplate() = default;
