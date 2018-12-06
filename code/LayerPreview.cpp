// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LayerPreview.hpp"

#include "Primitives.hpp"
#include "Resources.hpp"
#include "Preferences.hpp"
#include "Flat3DPVM.hpp"

#include <Magnum/GL/Renderer.h>

using namespace std;
using namespace Magnum;

static Vector3 v(float x, float y, float z)
{
    return { x, y, z };
}

LayerPreview::LayerPreview()
{
    std::vector<Triangle> solidTriangles = {
        // front
        { v(0, 0, 0), v(1, 0, 0), v(1, 1, 0) },
        { v(1, 1, 0), v(0, 1, 0), v(0, 0, 0) },
        { v(1, 0, 0), v(2, 0, 0), v(2, 1, 0) },
        { v(2, 1, 0), v(1, 1, 0), v(1, 0, 0) },


        // side
        { v(2, 0, 0), v(2.5f, 0.5f, 0), v(2.5f, 1.5f, 0) },
        { v(2.5f, 1.5f, 0), v(2, 1, 0), v(2, 0, 0) },
        { v(2.5f, 0.5f, 0), v(3, 1, 0), v(3, 2, 0) },
        { v(3, 2, 0), v(2.5f, 1.5f, 0), v(2.5f, 0.5f, 0) },

        // top
        { v(0, 1, 0), v(1, 1, 0), v(1.5f, 1.5f, 0) },
        { v(1.5f, 1.5f, 0), v(0.5f, 1.5f, 0), v(0, 1, 0) },
        { v(1, 1, 0), v(2, 1, 0), v(2.5f, 1.5f, 0) },
        { v(2.5f, 1.5f, 0), v(1.5f, 1.5f, 0), v(1, 1, 0) },

        { v(0.5f, 1.5, 0), v(1.5f, 1.5, 0), v(2, 2, 0) },
        { v(2, 2, 0), v(1, 2, 0), v(0.5f, 1.5f, 0) },
        { v(1.5f, 1.5f, 0), v(2.5f, 1.5f, 0), v(3, 2, 0) },
        { v(3, 2, 0), v(2, 2, 0), v(1.5f, 1.5f, 0) },
    };

    std::vector<LineSegment> wireSegments = {
        // horizontal
        { v(0, 0, 0), v(2, 0, 0) },
        { v(0, 1, 0), v(2, 1, 0) },
        { v(0.5f, 1.5f, 0), v(2.5f, 1.5f, 0) },
        { v(1, 2, 0), v(3, 2, 0) },

        // vertical
        { v(0, 0, 0), v(0, 1, 0) },
        { v(1, 0, 0), v(1, 1, 0) },
        { v(2, 0, 0), v(2, 1, 0) },
        { v(2.5f, 0.5f, 0), v(2.5f, 1.5f, 0) },
        { v(3, 1, 0), v(3, 2, 0) },

        // slanted
        { v(0, 1, 0), v(1, 2, 0) },
        { v(1, 1, 0), v(2, 2, 0) },
        { v(2, 1, 0), v(3, 2, 0) },
        { v(2, 0, 0), v(3, 1, 0) },
    };

    GL::Buffer wireBuffer;
    wireBuffer.setData(wireSegments);
    m_wire
        .setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(Int(wireSegments.size()) * 2)
        .addVertexBuffer(std::move(wireBuffer), 0,
            Shaders::Flat3D::Position{});

    GL::Buffer solidBuffer;
    solidBuffer.setData(solidTriangles);
    m_solid
        .setCount(Int(solidTriangles.size()) * 3)
        .addVertexBuffer(std::move(solidBuffer), 0,
            Shaders::Flat3D::Position{});
}

LayerPreview::~LayerPreview() = default;

void LayerPreview::draw(size_t numLayers) const
{
    GL::Renderer::setDepthMask(false);

    auto& shader = Resources::shaderFlat3D();
    auto pv = Matrix4::translation({ -1, -1, 0 }) * Matrix4::orthographicProjection({ 3.1f, 30 }, 0, 10);
    shader.setProjView(pv);

    for (uint32_t i = 0; i < numLayers; ++i)
    {
        shader.setModel(Matrix4::translation({ 0, 1.8f * i, 0 }));

        shader.setColor(Preferences::layerColor(i));
        m_solid.draw(shader);

        shader.setColor(Color4{ 0.7f, 0.7f, 0.7f, 1 });
        m_wire.draw(shader);
    }

    GL::Renderer::setDepthMask(true);
}