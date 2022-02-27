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

#include "BasicPit.hpp"
#include "BlockTemplate.hpp"

#include "Renderer.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-gfx.h"

#include <array>
#include <optional>

namespace
{

constexpr int MAX_GRID_SIZE = 5;

struct BlockGrid
{
    int size = 1;
    bool buf[MAX_GRID_SIZE][MAX_GRID_SIZE][MAX_GRID_SIZE];
};

struct BlockEData
{
    std::string name;
    BlockGrid grid;
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
    LayoutBlockBuilder m_layout;
    std::optional<CurBlockEState> m_curBlockEState;

    bool m_curBlockGeometryDirty = true;
    bool m_pitGeometryDirty = true;

    std::unique_ptr<BasicPit> m_pit;
    std::unique_ptr<BlockTemplate> m_block;

    ModeBlockBuilder() {}

    virtual const char* name() const override { return "Block Builder"; }

    virtual bool init() override
    {
        m_curBlockEState.emplace();
        m_curBlockEState->data.grid.size = 3;

        return true;
    }

    virtual void update(ms_t, ivec2 windowSize) override
    {
        m_layout.update(windowSize);

        if (!m_curBlockEState) return;
        auto& curBlockData = m_curBlockEState->data;
        auto& grid = curBlockData.grid;

        ImGui_BeginLayoutWindow(m_layout.blockData());
        if (ImGui::SliderInt("Grid", &grid.size, 1, MAX_GRID_SIZE))
        {
            m_curBlockGeometryDirty = true;
            m_pitGeometryDirty = true;
        }
        ImGui::End();

        ImGui_BeginLayoutWindow(m_layout.blockLayers());

        for (int z = 0; z < grid.size; ++z)
        {
            ImGui::Text("Layer %d", z);
            for (int y = 0; y < grid.size; ++y)
            {
                for (int x = 0; x < grid.size; ++x)
                {
                    ImGui::PushID(z*yama::sq(MAX_GRID_SIZE) + y*MAX_GRID_SIZE + x);
                    if (ImGui::Checkbox("", &grid.buf[z][y][x]))
                    {
                        m_curBlockGeometryDirty = true;
                    }
                    ImGui::PopID();
                    ImGui::SameLine();
                }
                ImGui::NewLine();
            }
            ImGui::NewLine();
        }

        ImGui::End();

        // auto& previewArea = m_layout.curBlockPreview();
        // auto minSize = std::min(previewArea.size.x, previewArea.size.y);
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
        m_pit.reset(new BasicPit(ivec3::uniform(m_curBlockEState->data.grid.size)));
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
        auto& previewArea = m_layout.blockPreview();
        auto minSize = std::min(previewArea.size.x, previewArea.size.y);
        sg_apply_viewport(previewArea.topLeft.x, previewArea.topLeft.y, minSize, minSize, true);
        m_pit->draw(r);
    }
};
} // namespace

AppModePtr MakeMode_BlockBuilder()
{
    return std::make_unique<ModeBlockBuilder>();
}
