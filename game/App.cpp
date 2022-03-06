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
#include "ModePanic.hpp"
#include "ModeLayoutTest.hpp"

#include "Renderer.hpp"

#include "lib/Time.hpp"
#include "lib/sokol-imgui.hpp"
#include "lib/sokol-glue.h"

#include <yama/vector4.hpp>

namespace
{

using clock = std::chrono::steady_clock;

struct InitialMode : public AppMode
{
    AppModePtr nextMode;
    InitialMode(AppModePtr mode) : nextMode(std::move(mode)) { setComplete(); }
    virtual const char* name() const override { return "initial"; }
    virtual AppModePtr getNextMode() override { return nextMode; }
};

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

        auto mode = MakeMode_BlockBuilder();
        //auto mode = MakeMode_Experimental();
        m_mode = std::make_shared<InitialMode>(std::move(mode));
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
        m_mode->update(dtms, screen);

        sg_begin_default_pass(&m_defaultPassAction, screen.x, screen.y);
        m_mode->defaultRender(screen);
        simgui_render();
        sg_end_pass();
        sg_commit();
    }

    void panic()
    {
        m_panicPending = true;
    }

    void immediatePanic()
    {
        puts("panic!");
        if (m_preservedModeOnPanic)
        {
            // error: panicking while we are in panic?
            return;
        }
        m_mode->deactivate();
        m_preservedModeOnPanic = m_mode;
        m_mode = MakeMode_Panic();
        m_mode->activate();
    }

    void checkForModeChange()
    {
        if (m_panicPending)
        {
            immediatePanic();
            return;
        }

        if (!m_mode->completed()) return; // no mode change

        m_mode->deactivate();
        m_mode = m_mode->getNextMode();
        if (!m_mode)
        {
            // TODO: default modes
            m_mode = MakeMode_Panic();
        }

        printf("Switching to mode %s\n", m_mode->name());
        if (!m_mode->activate())
        {
            immediatePanic();
            return;
        }
    }

    void onEvent(const sapp_event& event)
    {
        if (simgui_handle_event(&event)) return;
        m_mode->handleEvent(event);
    }

    clock::time_point m_time = {};

    sg_pass_action m_defaultPassAction = {};

    Renderer m_renderer;

    AppModePtr m_mode;

    // when this pointer is not null, we are in panic state
    // it holds the mode which was active when the panic state was initiated
    // (m_mode being ModePanic after that point)
    // we use this to make the guarantee to modes that no unexpected changes of state will happen
    // while they're active and generally allow them to be in control of their lifetime
    AppModePtr m_preservedModeOnPanic;

    // panic was initiated during this frame
    bool m_panicPending = false;
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
