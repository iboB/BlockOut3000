// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "StateBlockTest.hpp"

#include "App.hpp"
#include "AppState.hpp"

#include "lib/imgui.hpp"
#include "lib/sokol-app.h"

namespace
{

class StateBlockTest final : public AppState
{
public:
    BlockTemplatePtr m_block;
    AppStatePtr m_resumeState;

    virtual const char* name() const { return "block test"; }

    virtual void update(ms_t dt, ivec2 windowSize) override
    {
        ImGui::Begin("Block test");
        if (ImGui::Button("Done")) setComplete();
        ImGui::End();
    }

    virtual AppStatePtr getNextState() override { return m_resumeState; }
};

} // namespace

AppStatePtr MakeState_BlockTest(BlockTemplatePtr block, AppStatePtr resume)
{
    auto ret = std::make_shared<StateBlockTest>();
    ret->m_block = std::move(block);
    ret->m_resumeState = std::move(resume);
    return ret;
}