// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeLayoutTest.hpp"

#include "App.hpp"
#include "AppMode.hpp"

#include "LayoutExperimental.hpp"
#include "LayoutBlockBuilder.hpp"

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
        m_layouts.emplace_back(new LayoutBlockBuilder);
        m_layouts.emplace_back(new LayoutExperimental);
    }

    virtual const char* name() const override { return "Layout Test"; }

    virtual void defaultRender(ivec2 windowSize) override
    {
        auto cur = m_layouts[m_curLayoutIndex].get();
        cur->update(windowSize);
        for (auto& elem : cur->namedElements())
        {
            ImGui::SetNextWindowPos({float(elem->topLeft.x), float(elem->topLeft.y)});
            ImGui::SetNextWindowSize({float(elem->size.x), float(elem->size.y)});
            ImGui::Begin(elem->name.c_str());
            ImGui::End();
        }

        ImGui::Begin("Selector");
        auto selector = [](void* pdata, int index, const char** out) {
            auto& data = *static_cast<std::vector<std::unique_ptr<GUILayout>>*>(pdata);
            *out = data[index]->name();
            return true;
        };
        ImGui::ListBox("Layouts", &m_curLayoutIndex, selector, &m_layouts, int(m_layouts.size()), 10);
        ImGui::End();
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
    int m_curLayoutIndex = 0;
};
} // namespace

AppModePtr MakeMode_LayoutTest()
{
    return std::make_unique<ModeLayoutTest>();
}
