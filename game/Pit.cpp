#include "Pit.hpp"

#include "Vertex.hpp"
#include "Renderer.hpp"

#include <vector>

Pit::Pit(ivec3 size)
    : m_size(size)
{
    createBuffers();
}

Pit::~Pit() = default;

void Pit::draw(Renderer& r)
{
    {
        sg_apply_pipeline(r.solidTrisPipeline());
        sg_bindings b = {};
        b.vertex_buffers[0] = m_solid.vb;
        sg_apply_bindings(&b);

        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(m_projView));

        yama::vector4 solidColor = {0.066f, 0.066f, 0.15f, 1};
        sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, SG_RANGE(solidColor));

        sg_draw(0, m_solid.numElements, 1);
    }

    {
        sg_apply_pipeline(r.wirePipeline());
        sg_bindings b = {};
        b.vertex_buffers[0] = m_wire.vb;
        sg_apply_bindings(&b);

        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(m_projView));

        yama::vector4 wireColor = {0.13f, 0.30f, 0.15f, 1};
        sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, SG_RANGE(wireColor));

        sg_draw(0, m_wire.numElements, 1);
    }
}

void Pit::createBuffers()
{
    float w = float(m_size.x);
    float h = float(m_size.y);
    float d = float(m_size.z);

    using yama::constants;
    auto proj = matrix::perspective_fov_rh(constants::PI_HALF, 1, 1, 100);
    auto view = matrix::look_at_rh({0, 0, 1}, {0, 0, 0}, {0, 1, 0});
    auto pv = proj * view;

    // the z translation must be -d for a half-cube clip space and -2*d for a full-cube clip space
    m_projView = pv * matrix::scaling(2 / w, 2 / h, 2 / d) * matrix::translation(-w / 2, -h / 2, -d);

    Vertex solidVertices[] = {
        // bottom
        {0, 0, 0},
        {w, 0, 0},
        {w, h, 0},
        {w, h, 0},
        {0, h, 0},
        {0, 0, 0},

        // south
        {0, 0, 0},
        {0, 0, d},
        {w, 0, 0},
        {w, 0, 0},
        {0, 0, d},
        {w, 0, d},

        // east
        {w, 0, 0},
        {w, 0, d},
        {w, h, d},
        {w, h, d},
        {w, h, 0},
        {w, 0, 0},

        // north
        {0, h, 0},
        {w, h, 0},
        {w, h, d},
        {w, h, d},
        {0, h, d},
        {0, h, 0},

        // west
        {0, 0, 0},
        {0, h, 0},
        {0, 0, d},
        {0, 0, d},
        {0, h, 0},
        {0, h, d},
    };

    // make the pit box just a tiny bit larger, so that it doesn't z-fight the wires
    for (auto& v : solidVertices)
    {
        for (auto& coord : v.pos)
        {
            if (coord)
                coord += 0.01f;
            else
                coord -= 0.01f;
        }
    }

    m_solid = Mesh::create(solidVertices);


    std::vector<Vertex> wireVertices;
    wireVertices.reserve(2 * 3 * (m_size.x + 1) + 2 * 3 * (m_size.y + 1) + 2 * 4 * (m_size.z + 1));

    // concentric squares
    for (int i = 0; i < m_size.z + 1; ++i)
    {
        float d = float(i);
        wireVertices.insert(wireVertices.end(), {{0, 0, d}, {w, 0, d}, {w, 0, d}, {w, h, d}, {w, h, d}, {0, h, d}, {0, h, d}, {0, 0, d}});
    }

    // vertical U
    for (int i = 0; i < m_size.x + 1; ++i)
    {
        float w = float(i);
        wireVertices.insert(wireVertices.end(), {{w, 0, d}, {w, 0, 0}, {w, 0, 0}, {w, h, 0}, {w, h, 0}, {w, h, d}});
    }

    // horizontal U
    for (int i = 0; i < m_size.y + 1; ++i)
    {
        float h = float(i);
        wireVertices.insert(wireVertices.end(), {{0, h, d}, {0, h, 0}, {0, h, 0}, {w, h, 0}, {w, h, 0}, {w, h, d}});
    }

    m_wire = Mesh::create(wireVertices);
}
