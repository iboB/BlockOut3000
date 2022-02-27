// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeExperimental.hpp"

#include "App.hpp"
#include "AppMode.hpp"
#include "BasicPit.hpp"
#include "BlockTemplate.hpp"
#include "RotateI.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-app.h"

#include <yama/vector4.hpp>

#include <memory>

namespace
{
class ModeExperimental final : public AppMode
{
public:
    ModeExperimental()
        : m_pit({5, 5, 5})
    {
        m_blockElements = {{1, 0, 0}, {1, 1, 0}, {1, 2, 0}, {2, 2, 0}};
        recreateBlock();
    }

    void recreateBlock()
    {
        m_block.reset(new BlockTemplate("foo", 3, m_blockElements));
        m_block->ensurePhysicalData();
    }

    virtual const char* name() const override { return "Experimental"; }

    virtual bool init() override { return true; }

    virtual void update(ms_t, ivec2) override {}

    virtual void defaultRender(ivec2) override
    {
        auto& r = App::r();

        m_pit.draw(r);
        m_block->draw(r, m_pit.projView());
    }

    virtual bool handleEvent(const sapp_event& event) override
    {
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_ESCAPE)
        {
            sapp_quit();
            return true;
        }
        if (event.type == SAPP_EVENTTYPE_KEY_UP && event.key_code == SAPP_KEYCODE_SPACE)
        {
            for (auto& elem : m_blockElements)
            {
                elem = RotateY_CCW(elem, 3);
            }
            recreateBlock();
        }
        return false;
    }

    BasicPit m_pit;
    std::unique_ptr<BlockTemplate> m_block;
    std::vector<ivec3> m_blockElements;
};
} // namespace

AppModePtr MakeMode_Experimental()
{
    return std::make_unique<ModeExperimental>();
}
