// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModePanic.hpp"
#include "AppMode.hpp"

#include "lib/imgui.hpp"

namespace
{
class ModePanic : public AppMode
{
    virtual const char* name() const override { return "Panic"; }

    virtual void update(ms_t, ivec2) override
    {
        ImGui::Begin("Error");
        ImGui::Text("Fatal error!");
        ImGui::End();
    }
};
} // namespace

AppModePtr MakeMode_Panic()
{
    return std::make_shared<ModePanic>();
}
