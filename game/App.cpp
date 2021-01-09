#include "App.hpp"

#include "sokol-imgui.hpp"

#include <yama/vector4.hpp>

#include <chrono>

namespace
{

using clock = std::chrono::steady_clock;
using sec_t = std::chrono::duration<float>;

class AppImpl
{
public:
    void init()
    {
        m_time = clock::now();

        {
            sg_desc desc = {};
            sg_setup(&desc);
        }

        {
            m_defaultPassAction.colors[0].action = SG_ACTION_CLEAR;
            yama::vector4::attach_to_ptr(m_defaultPassAction.colors[0].val) = {0.2f, 0.4f, 0.2f, 1.0f};
            m_defaultPassAction.depth.action = SG_ACTION_CLEAR;
            m_defaultPassAction.depth.val = 1;
            m_defaultPassAction.stencil.action = SG_ACTION_DONTCARE;
        }

        {
            simgui_desc_t desc = {};
            simgui_setup(&desc);
        }
    }

    ~AppImpl()
    {
        simgui_shutdown();
        sg_shutdown();
    }

    void frame()
    {
        auto now = clock::now();
        auto dt = now - m_time;
        auto dtsec = std::chrono::duration_cast<sec_t>(now - m_time);
        if (dtsec.count() == 0) return;
        m_time = now;

        const int w = sapp_width();
        const int h = sapp_height();

        simgui_new_frame(w, h, dtsec.count());

        ImGui::ShowDemoWindow();

        sg_begin_default_pass(&m_defaultPassAction, w, h);
        simgui_render();
        sg_end_pass();
        sg_commit();
    }

    void onEvent(const sapp_event& event)
    {
        simgui_handle_event(&event);
    }

    clock::time_point m_time = {};

    sg_pass_action m_defaultPassAction = {};
};

AppImpl* theApp;

}


extern "C" {

extern sapp_desc sokol_main(int, char*[])
{
    sapp_desc desc = {};

    desc.init_cb = []() {
        theApp = new AppImpl;
        theApp->init();
    };
    desc.frame_cb = []() {
        theApp->frame();
    };
    desc.cleanup_cb = []() {
        delete theApp;
        theApp = nullptr;
    };
    desc.event_cb = [](const sapp_event* event) {
        theApp->onEvent(*event);
    };

    desc.width = 1400;
    desc.height = 900;
    desc.window_title = "BlockOut 3000";

    desc.enable_clipboard = true;

    return desc;
}
}
