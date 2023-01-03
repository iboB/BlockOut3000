// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "StateBlockBuilder.hpp"

#include "App.hpp"
#include "AppState.hpp"
#include "LayoutBlockBuilder.hpp"
#include "RotateI.hpp"
#include "BasicPit.hpp"
#include "BlockTemplate.hpp"
#include "Renderer.hpp"
#include "StateBlockTest.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-app.h"

#include <array>

namespace
{

constexpr int MAX_BLOCK_NAME_LENGTH = 20;
constexpr int MAX_GRID_SIZE = 5;

struct BlockData
{
    char name[MAX_BLOCK_NAME_LENGTH] = {};

    struct Grid
    {
        using Buf = std::array<std::array<std::array<bool, MAX_GRID_SIZE>, MAX_GRID_SIZE>, MAX_GRID_SIZE>;
        Buf buf = {};
        int size = 3;
    };
    Grid grid;
};

template <typename F>
void forEachSet(const BlockData::Grid& g, F f)
{
    for (int z = 0; z < g.size; ++z)
    {
        for (int y = 0; y < g.size; ++y)
        {
            for (int x = 0; x < g.size; ++x)
            {
                if (g.buf[z][y][x])
                {
                    f(ivec3{x, y, z});
                }
            }
        }
    }
}

struct BlockEditData
{
    BlockData block;
    struct HistoryItem
    {
        std::string label;
        BlockData block;
    };
    std::vector<HistoryItem> history;
    size_t historyPointer = 0;
};

struct BlockPhysicalData
{
    BlockData source;
    std::unique_ptr<BasicPit> pit;
    BlockTemplatePtr block;
};

void ImGui_BeginLayoutWindow(const GUILayout::NamedElement& elem)
{
    ImGui::SetNextWindowPos({float(elem.topLeft.x), float(elem.topLeft.y)});
    ImGui::SetNextWindowSize({float(elem.size.x), float(elem.size.y)});
    ImGui::Begin(elem.name.c_str());
}

class StateBlockBuilder final : public AppState, public std::enable_shared_from_this<StateBlockBuilder>
{
public:
    LayoutBlockBuilder m_layout;
    BlockEditData m_blockEditData;
    BlockPhysicalData m_physicalData;

    AppStatePtr m_nextState;

    StateBlockBuilder() {
        m_blockEditData = {};
        m_blockEditData.history.push_back({"empty", {}});

        // force recreation of pit with invalid size data
        m_physicalData.source.grid.size = 0;
    }

    virtual const char* name() const override { return "Block Builder"; }

    virtual bool activate() override
    {
        m_completed = false;
        m_nextState = {};
        return true;
    }

    virtual void update(ms_t, ivec2 windowSize) override
    {
        m_layout.update(windowSize);

        auto& curBlockData = m_blockEditData.block;
        auto& grid = curBlockData.grid;

        ImGui_BeginLayoutWindow(m_layout.sets());
        ImGui::End();

        ImGui_BeginLayoutWindow(m_layout.curSet());
        ImGui::End();

        ImGui_BeginLayoutWindow(m_layout.blockData());

        ImGui::InputTextWithHint("Name", "name", curBlockData.name, MAX_BLOCK_NAME_LENGTH);
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            addAction("rename to %s", curBlockData.name);
        }

        if (ImGui::SliderInt("Grid", &grid.size, 1, MAX_GRID_SIZE))
        {
            addAction("grid resized to %d", grid.size);
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
            addAction("shift x");
        }
        ImGui::SameLine();
        if (ImGui::Button("S y"))
        {
            for (int z = 0; z < grid.size; ++z)
            {
                auto gb = grid.buf[z].begin();
                std::rotate(gb, gb + grid.size - 1, gb + grid.size);
            }
            addAction("shift y");
        }
        ImGui::SameLine();
        if (ImGui::Button("S z"))
        {
            auto gb = grid.buf.begin();
            std::rotate(gb, gb + 1, gb + grid.size);
            addAction("shift z");
        }

        // rotates
        auto rotateGrid = [&](ivec3(*rotate)(ivec3, int)) {
            // copying grid for rotation
            // we could transpose in place but this is much more readable and perf doesn't matter that much here
            BlockData::Grid::Buf b2 = {};
            forEachSet(grid, [&](ivec3 v) {
                auto rv = rotate(v, grid.size);
                b2[rv.z][rv.y][rv.x] = true;
            });
            grid.buf = b2;
        };
        if (ImGui::Button("R x"))
        {
            rotateGrid(RotateX_CW);
            addAction("rotate x");
        }
        ImGui::SameLine();
        if (ImGui::Button("R y"))
        {
            rotateGrid(RotateY_CW);
            addAction("rotate y");
        }
        ImGui::SameLine();
        if (ImGui::Button("R z"))
        {
            rotateGrid(RotateZ_CW);
            addAction("rotate z");
        }

        ImGui::NewLine();

        if (ImGui::Button("Test..."))
        {
            m_nextState = MakeState_BlockTest(m_physicalData.block, shared_from_this());
            setComplete();
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
                        addAction("toggle %d;%d;%d", x, y, z);
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
        ImGui::BeginListBox("##actions", {-FLT_MIN, -FLT_MIN});
        for (size_t i = 0; i < m_blockEditData.history.size(); ++i)
        {
            const auto& item = m_blockEditData.history[i];
            auto& ptr = m_blockEditData.historyPointer;
            auto curSelected = i == ptr;
            ImGui::PushID(int(i));
            if (ImGui::Selectable(item.label.c_str(), curSelected))
            {
                if (!curSelected) // clicking on the same action again
                {
                    gotoAction(i);
                }
            }
            ImGui::PopID();
        }
        ImGui::EndListBox();
        ImGui::End();

        updatePhysicalData();
    }

    void updatePhysicalData()
    {
        const auto& logical = m_blockEditData.block;
        auto& physical = m_physicalData.source;

        bool pitDirty = logical.grid.size != physical.grid.size;
        bool blockDirty = pitDirty || logical.grid.buf != physical.grid.buf;

        if (!blockDirty && !pitDirty) return;

        auto& grid = physical.grid;

        physical = logical;
        if (pitDirty)
        {
            m_physicalData.pit.reset(new BasicPit(ivec3::uniform(grid.size)));
        }

        if (!blockDirty) return;

        std::vector<ivec3> elements;
        forEachSet(grid, [&](ivec3 v) {
            // note that y is inverted to match coordinate system
            v.y = grid.size - v.y - 1;
            elements.push_back(v);
        });

        if (elements.empty())
        {
            m_physicalData.block.reset();
        }
        else
        {
            m_physicalData.block.reset(new BlockTemplate(physical.name, grid.size, std::move(elements)));
            m_physicalData.block->ensurePhysicalData();
        }
    }

    void addAction(const char* fmt, ...)
    {
        auto& history = m_blockEditData.history;
        auto& ptr = m_blockEditData.historyPointer;

        // erase undone actions which are invalidated:
        if (ptr != history.size() - 1)
        {
            history.erase(history.begin() + ptr + 1, history.end());
        }

        // create label
        va_list args;
        va_start(args, fmt);
        va_list argscopy;
        va_copy(argscopy, args);

        const auto n = vsnprintf(nullptr, 0, fmt, args) + 1; // +1 extra space for '\0'

        std::string label(n, 0);
        vsnprintf(label.data(), label.size(), fmt, argscopy);
        va_end(args);

        // add action
        auto& newAction = history.emplace_back();
        newAction.label = label;
        newAction.block = m_blockEditData.block;
        ++ptr;
    }

    void defaultRender(ivec2) override
    {
        auto& r = App::r();
        auto& previewArea = m_layout.blockPreview();
        sg_apply_viewport(previewArea.topLeft.x, previewArea.topLeft.y, previewArea.size.x, previewArea.size.y, true);
        m_physicalData.pit->draw(r);
        if (m_physicalData.block) m_physicalData.block->draw(r, m_physicalData.pit->projView());
    }

    void gotoAction(size_t i)
    {
        auto& history = m_blockEditData.history;
        if (i >= history.size()) return;

        m_blockEditData.block = history[i].block;
        m_blockEditData.historyPointer = i;
    }

    virtual bool handleEvent(const sapp_event& event) override
    {
        if (event.type != SAPP_EVENTTYPE_KEY_UP) return false;
        if (event.modifiers != SAPP_MODIFIER_CTRL) return false;
        if (event.key_code == SAPP_KEYCODE_Z)
        {
            gotoAction(m_blockEditData.historyPointer - 1);
            return true;
        }
        else if (event.key_code == SAPP_KEYCODE_Y)
        {
            gotoAction(m_blockEditData.historyPointer + 1);
            return true;
        }
        return false;
    }

    virtual AppStatePtr getNextState() override { return m_nextState; }
};
} // namespace

AppStatePtr MakeState_BlockBuilder()
{
    return std::make_shared<StateBlockBuilder>();
}
