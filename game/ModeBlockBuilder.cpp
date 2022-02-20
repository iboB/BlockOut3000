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

#include "Pit.hpp"

#include "Renderer.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-gfx.h"

#include <array>
#include <optional>

namespace
{

constexpr int MAX_GRID_SIZE = 5;

struct BlockEData
{
    std::string name;
    int gridSize = 1;
    bool grid[MAX_GRID_SIZE*MAX_GRID_SIZE*MAX_GRID_SIZE] = {};
};

struct CurBlockEState
{
    BlockEData data;
    struct HistoryItem
    {
        std::string label;
        BlockEData data;
    };
    std::vector<HistoryItem> history;
    size_t historyPointer = 0;
};

void ImGui_BeginLayoutWindow(const GUILayout::NamedElement& elem)
{
    ImGui::SetNextWindowPos({float(elem.topLeft.x), float(elem.topLeft.y)});
    ImGui::SetNextWindowSize({float(elem.size.x), float(elem.size.y)});
    ImGui::Begin(elem.name.c_str());
}

class ModeBlockBuilder final : public AppMode
{
public:
    ModeBlockBuilder() {}

    virtual const char* name() const override { return "Block Builder"; }

    virtual bool init() override
    {
        m_curBlockEState.emplace();
        m_curBlockEState->data.gridSize = 3;

        return true;
    }

    virtual void update(ms_t, ivec2 windowSize) override
    {
        m_layout.update(windowSize);

        if (!m_curBlockEState) return;
        auto& curBlockData = m_curBlockEState->data;

        ImGui_BeginLayoutWindow(m_layout.curBlock());
        ImGui::End();

        ImGui_BeginLayoutWindow(m_layout.curBlockLayers());

        int i = 0;
        for (int z = 0; z < curBlockData.gridSize; ++z)
        {
            ImGui::Text("Layer %d", z);
            for (int y = 0; y < curBlockData.gridSize; ++y)
            {
                for (int x = 0; x < curBlockData.gridSize; ++x)
                {
                    ImGui::PushID(z*curBlockData.gridSize*curBlockData.gridSize + y*curBlockData.gridSize + x);
                    if (ImGui::Checkbox("", &curBlockData.grid[i]))
                    {
                        m_curBlockGeometryDirty = true;
                    }
                    ImGui::PopID();
                    ImGui::SameLine();
                    ++i;
                }
                ImGui::NewLine();
            }
            ImGui::NewLine();
        }

        ImGui::End();

        auto& previewArea = m_layout.curBlockPreview();
        auto minSize = std::min(previewArea.size.x, previewArea.size.y);
        m_previewTopLeft = previewArea.topLeft;
        m_previewSize = {minSize, minSize};
        // ImGui::SetNextWindowPos({float(previewArea.topLeft.x), float(previewArea.topLeft.y)});
        // ImGui::SetNextWindowSize({float(minSize), float(minSize)});
        // ImGui::Begin("Preview");
        // ImGui::End();

        updateBlock();
        updatePit();
    }

    void updatePit()
    {
        if (!m_pitGeometryDirty) return;
        m_pit.reset(new Pit(ivec3::uniform(m_curBlockEState->data.gridSize)));
        m_pitGeometryDirty = false;
    }

    void updateBlock()
    {
        if (!m_curBlockGeometryDirty) return;
        m_curBlockGeometryDirty = false;
    }

    void defaultRender(ivec2) override
    {
        auto& r = App::r();
        sg_apply_viewport(m_previewTopLeft.x, m_previewTopLeft.y, m_previewSize.x, m_previewSize.y, true);
        m_pit->draw(r);
    }

    LayoutBlockBuilder m_layout;
    std::optional<CurBlockEState> m_curBlockEState;

    ivec2 m_previewTopLeft;
    ivec2 m_previewSize;

    bool m_curBlockGeometryDirty = true;
    bool m_pitGeometryDirty = true;

    std::unique_ptr<Pit> m_pit;
};
} // namespace

AppModePtr MakeMode_BlockBuilder()
{
    return std::make_unique<ModeBlockBuilder>();
}
