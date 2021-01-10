// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Time.hpp"

#include "vec2.hpp"

extern "C" struct sapp_event;

class AppMode
{
public:
    virtual ~AppMode();

    virtual const char* name() const = 0;

    virtual bool init();
    virtual void update(ms_t dt);
    virtual void defaultRender(ivec2 windowSize);
    virtual bool handleEvent(const sapp_event& event);
};
