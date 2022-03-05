// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "App.hpp"

#include "AppMode.hpp"

#include "ModeExperimental.hpp"
#include "ModeBlockBuilder.hpp"
#include "ModeFatalError.hpp"
#include "ModeLayoutTest.hpp"

#include "Renderer.hpp"

#include "lib/Time.hpp"
#include "lib/sokol-imgui.hpp"
#include "lib/sokol-glue.h"

#include <yama/vector4.hpp>

namespace
{

using clock = std::chrono::steady_clock;

class AppImpl
{
public:
    void init()
    {
        m_time = clock::now();

        {
            m_defaultPassAction.colors[0].action = SG_ACTION_CLEAR;
            m_defaultPassAction.colors[0].value = {0.2f, 0.4f, 0.2f, 1.0f};
            m_defaultPassAction.depth.action = SG_ACTION_CLEAR;
            m_defaultPassAction.depth.value = 1;
            m_defaultPassAction.stencil.action = SG_ACTION_DONTCARE;
        }

        {
            simgui_desc_t desc = {};
            simgui_setup(&desc);
        }

        m_renderer.init();

        m_nextMode = MakeMode_BlockBuilder();
        //m_nextMode = MakeMode_Experimental();
    }

    ~AppImpl() { simgui_shutdown(); }

    void frame()
    {
        checkForModeChange();

        auto now = clock::now();
        auto dt = now - m_time;
        auto dtsec = std::chrono::duration_cast<sec_t>(dt);

        // too short frame to care
        if (dtsec.count() < yama::constants::EPSILON) return;

        m_time = now;

        ivec2 screen = {sapp_width(), sapp_height()};

        simgui_new_frame(simgui_frame_desc_t{
            .width = screen.x,
            .height = screen.y,
            .delta_time = dtsec.count(),
            .dpi_scale = sapp_dpi_scale()
        });

        auto dtms = std::chrono::duration_cast<ms_t>(dt);
        m_currentMode->update(dtms, screen);

        // ImGui::ShowDemoWindow();

        sg_begin_default_pass(&m_defaultPassAction, screen.x, screen.y);
        m_currentMode->defaultRender(screen);
        simgui_render();
        sg_end_pass();
        sg_commit();
    }

    void checkForModeChange()
    {
        if (!m_nextMode) return;

        // loop while we have a pending mode
        int fails = 0;
        while (true)
        {
            m_currentMode.reset();
            AppModePtr pending;
            pending.swap(m_nextMode);
            auto success = pending->init(); // this might set the next mode
            if (success)
            {
                printf("Switching to mode %s\n", pending->name());
                m_currentMode = std::move(pending);
                break;
            }
            else
            {
                printf("Error initializing mode %s\n", pending->name());
                ++fails;
            }

            if (fails == 5 || !m_nextMode) { m_nextMode = Make_Mode_FatalError(); }
        }
    }

    void onEvent(const sapp_event& event)
    {
        if (simgui_handle_event(&event)) return;
        m_currentMode->handleEvent(event);
    }

    clock::time_point m_time = {};

    sg_pass_action m_defaultPassAction = {};

    Renderer m_renderer;

    AppModePtr m_currentMode;
    AppModePtr m_nextMode;
};

AppImpl* theApp;

} // namespace

Renderer& App::r()
{
    return theApp->m_renderer;
}

extern "C"
{
    extern sapp_desc sokol_main(int, char*[])
    {
        sapp_desc desc = {};

        desc.init_cb = []() {
            {
                sg_desc desc = {};
                desc.context = sapp_sgcontext();
                sg_setup(&desc);
            }

            theApp = new AppImpl;
            theApp->init();
        };
        desc.frame_cb = []() { theApp->frame(); };
        desc.cleanup_cb = []() {
            delete theApp;
            theApp = nullptr;
            sg_shutdown();
        };
        desc.event_cb = [](const sapp_event* event) { theApp->onEvent(*event); };

        desc.width = 1400;
        desc.height = 900;
        desc.window_title = "BlockOut 3000";

        desc.enable_clipboard = true;

        return desc;
    }
}
