// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "App.hpp"

#include "AppState.hpp"

#include "StateExperimental.hpp"
#include "StateBlockBuilder.hpp"
#include "StatePanic.hpp"
#include "StateLayoutTest.hpp"

#include "Renderer.hpp"

#include "lib/Time.hpp"
#include "lib/sokol-imgui.hpp"
#include "lib/sokol-glue.h"

#include <yama/vector4.hpp>

namespace
{

using clock = std::chrono::steady_clock;

struct InitialState : public AppState
{
    AppStatePtr next;
    InitialState(AppStatePtr state) : next(std::move(state)) { setComplete(); }
    virtual const char* name() const override { return "initial"; }
    virtual AppStatePtr getNextState() override { return next; }
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

        //auto state = MakeState_BlockBuilder();
        //auto state = MakeState_Experimental();
        auto state = MakeState_LayoutTest();
        m_state = std::make_shared<InitialState>(std::move(state));
    }

    ~AppImpl() { simgui_shutdown(); }

    void frame()
    {
        checkForStateChange();

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
        m_state->update(dtms, screen);

        sg_begin_default_pass(&m_defaultPassAction, screen.x, screen.y);
        m_state->defaultRender(screen);
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
        if (m_preservedStateOnPanic)
        {
            // error: panicking while we are in panic?
            return;
        }
        m_state->deactivate();
        m_preservedStateOnPanic = m_state;
        m_state = MakeState_Panic();
        m_state->activate();
    }

    void checkForStateChange()
    {
        if (m_panicPending)
        {
            immediatePanic();
            return;
        }

        if (!m_state->completed()) return; // no state change

        m_state->deactivate();
        m_state = m_state->getNextState();
        if (!m_state)
        {
            // TODO: default states
            m_state = MakeState_Panic();
        }

        printf("Switching to state %s\n", m_state->name());
        if (!m_state->activate())
        {
            immediatePanic();
            return;
        }
    }

    void onEvent(const sapp_event& event)
    {
        if (simgui_handle_event(&event)) return;
        m_state->handleEvent(event);
    }

    clock::time_point m_time = {};

    sg_pass_action m_defaultPassAction = {};

    Renderer m_renderer;

    AppStatePtr m_state;

    // when this pointer is not null, we are in panic state
    // it holds the state which was active when the panic state was initiated
    // (m_state being StatePanic after that point)
    // we use this to make the guarantee to states that no unexpected changes of state will happen
    // while they're active and generally allow them to be in control of their lifetime
    AppStatePtr m_preservedStateOnPanic;

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
