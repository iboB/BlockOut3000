// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeExperimental.hpp"

#include "AppMode.hpp"
#include "App.hpp"
#include "Cube.hpp"
#include "Renderer.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-app.h"

#include <yama/vector4.hpp>
#include <yama/matrix4x4.hpp>

namespace
{
class ModeExperimental : public AppMode
{
    virtual const char* name() const override { return "Experimental"; }

    virtual bool init() override
    {
        {
            sg_buffer_desc desc = {};
            desc.data.ptr = Cube::instance().triangles().data();
            desc.data.size = Cube::instance().triangles().size() * sizeof(Triangle);
            m_buf = sg_make_buffer(desc);
        }
        return true;
    }

    virtual void update(ms_t) override
    {
    }

    virtual void defaultRender(ivec2 windowSize) override
    {
        sg_apply_pipeline(App::r().solidTrisPipeline());
        sg_bindings b = {};
        b.vertex_buffers[0] = m_buf;
        sg_apply_bindings(&b);

        auto proj = yama::matrix::perspective_fov_rh(yama::constants::PI_HALF, 1, 1, 100);
        auto view = yama::matrix::look_at_rh({1, 1, 3.5f}, {1, 1, 0}, {0, 1, 0});
        auto pv = proj * view;
        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(pv));

        yama::vector4 color = {1, 0, 0, 1};
        sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, SG_RANGE(color));

        sg_draw(0, Cube::instance().triangles().size() * 3, 1);
    }

    virtual bool handleEvent(const sapp_event& event)
    {
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_ESCAPE)
        {
            sapp_quit();
            return true;
        }
        return false;
    }

    sg_buffer m_buf;
};
}

AppModePtr MakeMode_Experimental()
{
    return std::make_unique<ModeExperimental>();
}
