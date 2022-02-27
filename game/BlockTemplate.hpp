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
#include "lib/matrix.hpp"

#include <vector>
#include <string>

class Renderer;

class BlockTemplate
{
public:
    // logical data
    std::string m_name;
    int m_grid;
    std::vector<ivec3> m_elements;

    // physical data
    Mesh m_solid;
    Mesh m_wire;

public:
    BlockTemplate(std::string name, int grid, std::vector<ivec3> elements);
    ~BlockTemplate();

    BlockTemplate(const BlockTemplate&) = delete;
    BlockTemplate& operator=(const BlockTemplate&) = delete;
    BlockTemplate(BlockTemplate&&) = delete;
    BlockTemplate& operator=(BlockTemplate&&) = delete;

    const std::string& name() const { return m_name; }
    int grid() const { return m_grid; }
    const std::vector<ivec3> elements() const { return m_elements; }

    void draw(Renderer& r, const matrix& pvm);

    // will create physical data if not present
    // used to lazy initialize templates
    void ensurePhysicalData();
};
