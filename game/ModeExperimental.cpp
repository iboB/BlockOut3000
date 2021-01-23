// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "ModeExperimental.hpp"
#include "AppMode.hpp"

#include "lib/imgui.hpp"

namespace
{
class ModeExperimental : public AppMode
{
    virtual const char* name() const override { return "Experimental"; }

    virtual bool init() override
    {
        return true;
    }

    virtual void update(ms_t) override
    {
    }
};
}

AppModePtr MakeMode_Experimental()
{
    return std::make_unique<ModeExperimental>();
}
