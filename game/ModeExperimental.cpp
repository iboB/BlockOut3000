// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeExperimental.hpp"

#include "App.hpp"
#include "AppMode.hpp"
#include "Cube.hpp"
#include "Pit.hpp"
#include "Renderer.hpp"
#include "Vertex.hpp"

#include "lib/Mesh.hpp"
#include "lib/imgui.hpp"
#include "lib/sokol-app.h"
#include "lib/sokol-gfx-utils.hpp"

#include <yama/matrix4x4.hpp>
#include <yama/vector4.hpp>

#include <map>

namespace
{
class ModeExperimental final : public AppMode
{
public:
    ModeExperimental()
        : m_pit({5, 5, 5})
    {
        m_blockElements = {{1, 0, 0}, {1, 1, 0}, {1, 2, 0}, {2, 2, 0}};
        recreateBlock();
    }

    void recreateBlock()
    {
        std::map<LineSegment, int, PrimitiveStrictWeakOrdering> allSegments;
        std::map<Triangle, int, PrimitiveStrictWeakOrdering> allTriangles;

        auto& cube = Cube::instance();
        for (auto& ielem : m_blockElements)
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

        m_blockWire = Mesh::create(wireSegments);
        m_blockWire.numElements *= 2;

        std::vector<Triangle> triangles;
        triangles.reserve(allTriangles.size());

        for (auto triNum : allTriangles)
        {
            if (triNum.second == 1) // only unique ones
            {
                triangles.push_back(triNum.first);
            }
        }

        m_blockSolid = Mesh::create(triangles);
        m_blockSolid.numElements *= 3;
    }

    virtual const char* name() const override { return "Experimental"; }

    virtual bool init() override { return true; }

    virtual void update(ms_t, ivec2) override {}

    virtual void defaultRender(ivec2) override
    {
        auto& r = App::r();

        m_pit.draw(r);

        {
            sg_apply_pipeline(r.alphaBackPipeline());
            sg_bindings b = {};
            b.vertex_buffers[0] = m_blockSolid.vb;
            sg_apply_bindings(&b);

            sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, sg_make_range(m_pit.projView()));

            yama::vector4 solidColor = {0.9f, 0.5f, 0.5f, 0.1f};
            sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, sg_make_range(solidColor));

            sg_draw(0, m_blockSolid.numElements, 1);
        }

        {
            sg_apply_pipeline(r.alphaFrontPipeline());
            sg_bindings b = {};
            b.vertex_buffers[0] = m_blockSolid.vb;
            sg_apply_bindings(&b);

            sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, sg_make_range(m_pit.projView()));

            yama::vector4 solidColor = {0.9f, 0.5f, 0.5f, 0.1f};
            sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, sg_make_range(solidColor));

            sg_draw(0, m_blockSolid.numElements, 1);
        }

        {
            sg_apply_pipeline(r.wirePipeline());
            sg_bindings b = {};
            b.vertex_buffers[0] = m_blockWire.vb;
            sg_apply_bindings(&b);

            sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, sg_make_range(m_pit.projView()));

            yama::vector4 wireColor = {1, 1, 1, 1};
            sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, sg_make_range(wireColor));

            sg_draw(0, m_blockWire.numElements, 1);
        }
    }

    virtual bool handleEvent(const sapp_event& event) override
    {
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_ESCAPE)
        {
            sapp_quit();
            return true;
        }
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_SPACE)
        {
            for (auto& elem : m_blockElements)
            {
                auto copy = elem;
                //elem.x = copy.y;
                //elem.y = 3 - 1 - copy.x;
                elem.x = 4 - 1 - copy.y;
                elem.y = copy.x;
            }
            recreateBlock();
        }
        return false;
    }

    Pit m_pit;

    Mesh m_blockSolid;
    Mesh m_blockWire;
    std::vector<ivec3> m_blockElements;
};
} // namespace

AppModePtr MakeMode_Experimental()
{
    return std::make_unique<ModeExperimental>();
}
