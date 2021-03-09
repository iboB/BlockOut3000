// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
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

#include "lib/Mesh.hpp"
#include "lib/imgui.hpp"
#include "lib/sokol-app.h"

#include <yama/matrix4x4.hpp>
#include <yama/vector4.hpp>

namespace
{
class ModeExperimental : public AppMode
{
public:
    ModeExperimental()
        : m_pit({5, 5, 5})
    {}

    virtual const char* name() const override { return "Experimental"; }

    virtual bool init() override { return true; }

    virtual void update(ms_t) override {}

    virtual void defaultRender(ivec2 windowSize) override { m_pit.draw(App::r()); }

    virtual bool handleEvent(const sapp_event& event)
    {
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_ESCAPE)
        {
            sapp_quit();
            return true;
        }
        return false;
    }

    Pit m_pit;
};
} // namespace

AppModePtr MakeMode_Experimental()
{
    return std::make_unique<ModeExperimental>();
}
