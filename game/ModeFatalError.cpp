// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeFatalError.hpp"
#include "AppMode.hpp"

#include "lib/imgui.hpp"

namespace
{
class ModeFatalError : public AppMode
{
    virtual const char* name() const override { return "Fatal error"; }

    virtual void update(ms_t) override
    {
        ImGui::Begin("Error");
        ImGui::Text("Fatal error!");
        ImGui::End();
    }
};
} // namespace

AppModePtr Make_Mode_FatalError()
{
    return std::make_unique<ModeFatalError>();
}
