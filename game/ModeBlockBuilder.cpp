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
#include "RotateI.hpp"
#include "BasicPit.hpp"
#include "BlockTemplate.hpp"
#include "Renderer.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-gfx.h"

#include <array>
#include <optional>

namespace
{

constexpr int MAX_BLOCK_NAME_LENGTH = 20;
constexpr int MAX_GRID_SIZE = 5;

using GridBuf = std::array<std::array<std::array<bool, MAX_GRID_SIZE>, MAX_GRID_SIZE>, MAX_GRID_SIZE>;

struct BlockGrid
{
    int size = 1;
    GridBuf buf = {};

    template <typename F>
    void eachSet(F f) const
    {
        for (int z = 0; z < size; ++z)
        {
            for (int y = 0; y < size; ++y)
            {
                for (int x = 0; x < size; ++x)
                {
                    if (buf[z][y][x])
                    {
                        f(ivec3{x, y, z});
                    }
                }
            }
        }
    }
};

struct BlockEData
{
    char name[MAX_BLOCK_NAME_LENGTH] = {};
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
        m_curBlockEState->history.push_back({"empty", {}});

        return true;
    }

    virtual void update(ms_t, ivec2 windowSize) override
    {
        m_layout.update(windowSize);

        if (!m_curBlockEState) return;
        auto& curBlockData = m_curBlockEState->data;
        auto& grid = curBlockData.grid;

        ImGui_BeginLayoutWindow(m_layout.blockData());

        ImGui::InputTextWithHint("Name", "name", curBlockData.name, MAX_BLOCK_NAME_LENGTH);

        if (ImGui::SliderInt("Grid", &grid.size, 1, MAX_GRID_SIZE))
        {
            m_curBlockGeometryDirty = true;
            m_pitGeometryDirty = true;
        }

        // shifts
        if (ImGui::Button("S x"))
        {
            for (int z = 0; z < grid.size; ++z)
            {
                for (int y = 0; y < grid.size; ++y)
                {
                    auto rb = grid.buf[z][y].begin();
                    std::rotate(rb, rb + grid.size - 1, rb + grid.size);
                }
            }
            m_curBlockGeometryDirty = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("S y"))
        {
            for (int z = 0; z < grid.size; ++z)
            {
                auto gb = grid.buf[z].begin();
                std::rotate(gb, gb + grid.size - 1, gb + grid.size);
            }
            m_curBlockGeometryDirty = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("S z"))
        {
            auto gb = grid.buf.begin();
            std::rotate(gb, gb + 1, gb + grid.size);
            m_curBlockGeometryDirty = true;
        }

        // rotates
        // copying grid for rotation
        // we could rotate in place but this is much more readable and perf doesn't matter that much here
        auto rotateCopy = [&](ivec3(*rotate)(ivec3, int)) {
            GridBuf b2 = {};
            grid.eachSet([&](ivec3 v) {
                auto rv = rotate(v, grid.size);
                b2[rv.z][rv.y][rv.x] = true;
            });
            grid.buf = b2;
            m_curBlockGeometryDirty = true;
        };
        if (ImGui::Button("R x"))
        {
            rotateCopy(RotateX_CW);
        }
        ImGui::SameLine();
        if (ImGui::Button("R y"))
        {
            rotateCopy(RotateY_CW);
        }
        ImGui::SameLine();
        if (ImGui::Button("R z"))
        {
            rotateCopy(RotateZ_CW);
        }

        ImGui::NewLine();

        if (ImGui::Button("Test..."))
        {

        }

        if (ImGui::Button("Save"))
        {

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

        ImGui_BeginLayoutWindow(m_layout.undoRedo());
        ImGui::PushID("actions box");
        ImGui::BeginListBox("");
        for (size_t i = 0; i < m_curBlockEState->history.size(); ++i)
        {
            auto& item = m_curBlockEState->history[i];
            auto curSelected = i == m_curBlockEState->historyPointer;
            if (ImGui::Selectable(item.label.c_str(), curSelected))
            {
                if (curSelected) continue;
            }
        }
        ImGui::EndListBox();
        ImGui::PopID();
        ImGui::End();

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

        std::vector<ivec3> elements;
        auto& curBlockData = m_curBlockEState->data;
        auto& grid = curBlockData.grid;
        curBlockData.grid.eachSet([&](ivec3 v) {
            // note that y is inverted to match coordinate system
            v.y = grid.size - v.y - 1;
            elements.push_back(v);
        });

        if (elements.empty())
        {
            m_block.reset();
        }
        else
        {
            m_block.reset(new BlockTemplate(curBlockData.name, curBlockData.grid.size, std::move(elements)));
            m_block->ensurePhysicalData();
        }

        m_curBlockGeometryDirty = false;
    }

    void defaultRender(ivec2) override
    {
        auto& r = App::r();
        auto& previewArea = m_layout.blockPreview();
        auto minSize = std::min(previewArea.size.x, previewArea.size.y);
        sg_apply_viewport(previewArea.topLeft.x, previewArea.topLeft.y, minSize, minSize, true);
        m_pit->draw(r);
        if (m_block) m_block->draw(r, m_pit->projView());
    }
};
} // namespace

AppModePtr MakeMode_BlockBuilder()
{
    return std::make_unique<ModeBlockBuilder>();
}
