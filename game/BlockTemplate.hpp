// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/vec3.hpp"
#include "lib/Mesh.hpp"

#include <vector>
#include <string>
#include <cstdint>

class BlockTemplate
{
public:
private:
    // logical data
    std::string m_name;
    std::vector<ivec3> m_elements;
    uint32_t m_grid;

    // physical data
    Mesh m_solid;
    Mesh m_wire;
};
