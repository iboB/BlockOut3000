// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "BlockTemplate.hpp"

#include "Primitives.hpp"
#include "Cube.hpp"

#include "lib/sokol-gfx-utils.hpp"

#include "Renderer.hpp"

#include <yama/vector4.hpp>

#include <map>

BlockTemplate::BlockTemplate(std::string name, uint32_t gridSize, std::vector<ivec3> elements)
    : m_name(std::move(name))
    , m_grid(gridSize)
    , m_elements(std::move(elements))
{}

BlockTemplate::~BlockTemplate() = default;

void BlockTemplate::draw(Renderer& r, const matrix& pvm)
{
    {
        sg_apply_pipeline(r.alphaBackPipeline());
        sg_bindings b = {};
        b.vertex_buffers[0] = m_solid.vb;
        sg_apply_bindings(&b);

        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, sg_make_range(pvm));

        yama::vector4 solidColor = {0.9f, 0.5f, 0.5f, 0.1f};
        sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, sg_make_range(solidColor));

        sg_draw(0, m_solid.numElements, 1);
    }

    {
        sg_apply_pipeline(r.alphaFrontPipeline());
        sg_bindings b = {};
        b.vertex_buffers[0] = m_solid.vb;
        sg_apply_bindings(&b);

        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, sg_make_range(pvm));

        yama::vector4 solidColor = {0.9f, 0.5f, 0.5f, 0.1f};
        sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, sg_make_range(solidColor));

        sg_draw(0, m_solid.numElements, 1);
    }

    {
        sg_apply_pipeline(r.wirePipeline());
        sg_bindings b = {};
        b.vertex_buffers[0] = m_wire.vb;
        sg_apply_bindings(&b);

        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, sg_make_range(pvm));

        yama::vector4 wireColor = {1, 1, 1, 1};
        sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, sg_make_range(wireColor));

        sg_draw(0, m_wire.numElements, 1);
    }
}

void BlockTemplate::ensurePhysicalData()
{
    if (m_solid.valid())
    {
        assert(m_wire.valid());
        return;
    }
    assert(!m_wire.valid());

    std::map<LineSegment, int, PrimitiveStrictWeakOrdering> allSegments;
    std::map<Triangle, int, PrimitiveStrictWeakOrdering> allTriangles;

    auto& cube = Cube::instance();
    for (auto& ielem : m_elements)
    {
        auto elem = vector_cast<vec3>(ielem);
        for (auto seg : cube.wireSegments())
        {
            seg[0] += elem;
            seg[1] += elem;
            ++allSegments[seg];
        }

        for (Triangle tri : cube.triangles())
        {
            tri[0] += elem;
            tri[1] += elem;
            tri[2] += elem;

            ++allTriangles[tri];
        }
    }

    std::vector<LineSegment> wireSegments;
    wireSegments.reserve(allSegments.size());

    for (auto segNum : allSegments)
    {
        if (segNum.second & 1) // odd number of encounters => we want it
        {
            wireSegments.push_back(segNum.first);
        }
    }

    m_wire = Mesh::create(wireSegments);
    m_wire.numElements *= 2;

    std::vector<Triangle> triangles;
    triangles.reserve(allTriangles.size());

    for (auto triNum : allTriangles)
    {
        if (triNum.second == 1) // only unique ones
        {
            triangles.push_back(triNum.first);
        }
    }

    m_solid = Mesh::create(triangles);
    m_solid.numElements *= 3;
}
