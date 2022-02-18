// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeBlockBuilder.hpp"

#include "App.hpp"
#include "AppMode.hpp"
#include "LayoutBlockBuilder.hpp"

#include "lib/imgui.hpp"

namespace
{
class ModeBlockBuilder final : public AppMode
{
public:
    ModeBlockBuilder() {}

    virtual const char* name() const override { return "Block Builder"; }

    virtual bool init() override { return true; }

    virtual void update(ms_t, ivec2) override {}

    virtual void defaultRender(ivec2 windowSize) override
    {
        m_layout.update(windowSize);
        auto& layersBox = m_layout.layers();
        ImGui::SetNextWindowPos({float(layersBox.topLeft.x), float(layersBox.topLeft.y)});
        ImGui::SetNextWindowSize({float(layersBox.size.x), float(layersBox.size.y)});
        ImGui::Begin(layersBox.name.c_str());

        bool b = false;
        for (int z = 0; z < m_gridSize; ++z) {
            ImGui::Text("Layer %d", z);
            for (int y = 0; y < m_gridSize; ++y) {
                for (int x = 0; x < m_gridSize; ++x) {
                    ImGui::PushID(z*m_gridSize*m_gridSize + y*m_gridSize + x);
                    ImGui::Checkbox("", &b);
                    ImGui::SameLine();
                    ImGui::PopID();
                }
                ImGui::NewLine();
            }
            ImGui::NewLine();
        }

        ImGui::End();
    }

    virtual bool handleEvent(const sapp_event&) override { return false; }

    LayoutBlockBuilder m_layout;
    int m_gridSize = 3;
};
} // namespace

AppModePtr MakeMode_BlockBuilder()
{
    return std::make_unique<ModeBlockBuilder>();
}
