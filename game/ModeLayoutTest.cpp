// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeLayoutTest.hpp"

#include "App.hpp"
#include "AppMode.hpp"

#include "LayoutExperimental.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-app.h"

namespace
{
class ModeLayoutTest final : public AppMode
{
public:
    ModeLayoutTest()
    {
        // add layouts to test with
        m_layouts.emplace_back(new LayoutExperimental);

        m_curLayout = m_layouts.front().get();
    }

    virtual const char* name() const override { return "Layout Test"; }

    virtual void defaultRender(ivec2 windowSize) override
    {
        m_curLayout->update(windowSize);
        for (auto& elem : m_curLayout->namedElements())
        {
            ImGui::SetNextWindowPos({float(elem->topLeft.x), float(elem->topLeft.y)});
            ImGui::SetNextWindowSize({float(elem->size.x), float(elem->size.y)});
            ImGui::Begin(elem->name.c_str());
            ImGui::End();
        }
    }

    virtual bool handleEvent(const sapp_event& event) override
    {
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_ESCAPE)
        {
            sapp_quit();
            return true;
        }
        return false;
    }

    std::vector<std::unique_ptr<GUILayout>> m_layouts;
    GUILayout* m_curLayout;
};
} // namespace

AppModePtr MakeMode_LayoutTest()
{
    return std::make_unique<ModeLayoutTest>();
}
