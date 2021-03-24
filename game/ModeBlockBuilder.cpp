// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeBlockBuilder.hpp"

#include "App.hpp"
#include "AppMode.hpp"

namespace
{
class ModeBlockBuilder final : public AppMode
{
public:
    ModeBlockBuilder() {}

    virtual const char* name() const override { return "Block Builder"; }

    virtual bool init() override { return true; }

    virtual void update(ms_t) override {}

    virtual void defaultRender(ivec2) override {}

    virtual bool handleEvent(const sapp_event& event) override { return false; }
};
} // namespace

AppModePtr MakeMode_BlockBuilder()
{
    return std::make_unique<ModeBlockBuilder>();
}
