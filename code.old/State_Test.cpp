// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "State_Test.hpp"
#include "AppState.hpp"

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Math/Matrix4.h>

#include <ctime>
#include <cassert>

using namespace Magnum;

namespace
{

// default implementations
class TestState : public AppState
{
    GL::Mesh _mesh;
    Shaders::VertexColor3D _shader;

    const char* name() const { return "test"; }
    bool initialize() override
    {
        using namespace Math::Literals;

        struct TriangleVertex {
            Vector3 position;
            Color3 color;
        };
        const TriangleVertex data[]{
            {{-0.5f, -0.5f, 0}, 0xff0000_rgbf},    /* Left vertex, red color */
            {{ 0.5f, -0.5f, 0}, 0x00ff00_rgbf},    /* Right vertex, green color */
            {{ 0.0f,  0.5f, 0}, 0x0000ff_rgbf}     /* Top vertex, blue color */
        };

        GL::Buffer buffer;
        buffer.setData(data);

        _mesh.setCount(3)
            .addVertexBuffer(std::move(buffer), 0,
                Shaders::VertexColor3D::Position{},
                Shaders::VertexColor3D::Color3{});

        return true;
    }

    void cleanup() override
    {
    }

    void update(uint32_t) override
    {
    }

    void draw() override
    {
        GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
        auto proj = Matrix4::perspectiveProjection(Rad{ Constants::piHalf() }, 1, 1, 100);
        auto view = Matrix4::lookAt({ 2, 0, -1.5f }, { 0, 0, 0 }, { 0, 1, 0 }).invertedRigid();
        _shader.setTransformationProjectionMatrix(proj * view);

        _mesh.draw(_shader);
    }
};

}

std::unique_ptr<AppState> State_Test()
{
    return std::make_unique<TestState>();
}
