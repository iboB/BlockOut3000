// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "FigureTemplate.hpp"

#include "CubeTemplate.hpp"
#include "Flat3DPVM.hpp"
#include "Resources.hpp"

#include <Magnum/GL/Renderer.h>

#include <map>

using namespace Magnum;
using namespace std;

FigureTemplate::FigureTemplate() = default;
FigureTemplate::~FigureTemplate() = default;

void FigureTemplate::preparePhysicalData()
{
    if(m_hasPhysicalData)
    {
        return; // already prepared
    }

    const CubeTemplate& theCube = Resources::cubeTemplate();

    map<LineSegment, int, PrimitiveStrictWeakOrdering> allSegments;
    map<Triangle, int, PrimitiveStrictWeakOrdering> allTriangles;
    for(auto ielem : m_elements)
    {
        auto elem = Vector3{ielem};

        for(LineSegment seg : theCube.wireSegments())
        {
            seg.at(0) += elem;
            seg.at(1) += elem;

            ++allSegments[seg];
        }

        for(Triangle tri : theCube.triangles())
        {
            tri.at(0) += elem;
            tri.at(1) += elem;
            tri.at(2) += elem;

            ++allTriangles[tri];
        }
    }


    std::vector<LineSegment> wireSegments;
    wireSegments.reserve(allSegments.size());

    for(auto segNum : allSegments)
    {
        if(segNum.second & 1) // odd number of encounters => we want it
        {
            wireSegments.push_back(segNum.first);
        }
    }

    std::vector<Triangle> triangles;
    triangles.reserve(allTriangles.size());

    for(auto triNum : allTriangles)
    {
        if(triNum.second == 1) // only unique ones
        {
            triangles.push_back(triNum.first);
        }
    }

    // create buffers
    GL::Buffer wireBuffer;
    wireBuffer.setData(wireSegments);
    m_wire
        .setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(Int(wireSegments.size()) * 2)
        .addVertexBuffer(std::move(wireBuffer), 0,
            Shaders::Flat3D::Position{});

    GL::Buffer solidBuffer;
    solidBuffer.setData(triangles);
    m_solid
        .setCount(Int(triangles.size()) * 3)
        .addVertexBuffer(std::move(solidBuffer), 0,
            Shaders::Flat3D::Position{});

    m_hasPhysicalData = true;
}


void FigureTemplate::draw(const Color4& solidColor, const Color4& wireColor) const
{
    auto& shader = Resources::shaderFlat3D();

    GL::Renderer::setDepthMask(false);

    shader.setColor(wireColor);
    m_wire.draw(shader);

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::setBlendFunction(
        GL::Renderer::BlendFunction::SourceAlpha,
        GL::Renderer::BlendFunction::OneMinusSourceAlpha
    );

    shader.setColor(solidColor);
    GL::Renderer::setFaceCullingMode(GL::Renderer::PolygonFacing::Front);
    m_solid.draw(shader);
    GL::Renderer::setFaceCullingMode(GL::Renderer::PolygonFacing::Back);
    m_solid.draw(shader);

    GL::Renderer::disable(GL::Renderer::Feature::Blending);
    GL::Renderer::setDepthMask(true);
}
