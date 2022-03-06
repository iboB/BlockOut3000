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
#include "lib/sokol-app.h"

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

    virtual bool activate() override
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
            m_curBlockGeometryDirty = true;
            m_pitGeometryDirty = true;
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
            m_curBlockGeometryDirty = true;
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
            m_curBlockGeometryDirty = true;
            addAction("shift y");
        }
        ImGui::SameLine();
        if (ImGui::Button("S z"))
        {
            auto gb = grid.buf.begin();
            std::rotate(gb, gb + 1, gb + grid.size);
            m_curBlockGeometryDirty = true;
            addAction("shift z");
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
            addAction("rotate x");
        }
        ImGui::SameLine();
        if (ImGui::Button("R y"))
        {
            rotateCopy(RotateY_CW);
            addAction("rotate y");
        }
        ImGui::SameLine();
        if (ImGui::Button("R z"))
        {
            rotateCopy(RotateZ_CW);
            addAction("rotate z");
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
        for (size_t i = 0; i < m_curBlockEState->history.size(); ++i)
        {
            const auto& item = m_curBlockEState->history[i];
            auto& ptr = m_curBlockEState->historyPointer;
            auto curSelected = i == ptr;
            ImGui::PushID(i);
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

    void addAction(const char* fmt, ...)
    {
        auto& history = m_curBlockEState->history;
        auto& ptr = m_curBlockEState->historyPointer;

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
        newAction.data = m_curBlockEState->data;
        ++ptr;
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

    void gotoAction(size_t i)
    {
        auto& history = m_curBlockEState->history;
        if (i >= history.size()) return;

        m_curBlockEState->data = history[i].data;
        m_curBlockEState->historyPointer = i;

        // we could try to compare states and determine which of these (if any) is needed exactly,
        // but why bother? We don't care about perf here
        m_curBlockGeometryDirty = true;
        m_pitGeometryDirty = true;
    }

    virtual bool handleEvent(const sapp_event& event) override
    {
        if (event.type != SAPP_EVENTTYPE_KEY_UP) return false;
        if (event.modifiers != SAPP_MODIFIER_CTRL) return false;
        if (event.key_code == SAPP_KEYCODE_Z)
        {
            gotoAction(m_curBlockEState->historyPointer - 1);
            return true;
        }
        else if (event.key_code == SAPP_KEYCODE_Y)
        {
            gotoAction(m_curBlockEState->historyPointer + 1);
            return true;
        }
        return false;
    }
};
} // namespace

AppModePtr MakeMode_BlockBuilder()
{
    return std::make_shared<ModeBlockBuilder>();
}
