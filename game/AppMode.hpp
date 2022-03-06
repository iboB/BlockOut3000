// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once
#include "AppModePtr.hpp"

#include "lib/Time.hpp"
#include "lib/vec2.hpp"

extern "C" struct sapp_event;

class AppMode
{
public:
    virtual ~AppMode();

    // for debugging purposes
    virtual const char* name() const = 0;

    // shows that the mode has completed its purpose
    // and can be released by the application
    // and substituted with a pending mode
    bool completed() const { return m_completed; }

    // app flow
    virtual bool activate();
    virtual void update(ms_t dt, ivec2 windowSize);
    virtual void defaultRender(ivec2 windowSize);
    virtual bool handleEvent(const sapp_event& event);
    virtual void deactivate();

    // optionally override to provide a next mode when modes are changed
    // may be called after deactivate!
    virtual AppModePtr getNextMode();

protected:
    void setComplete() { m_completed = true; }
    bool m_completed = false;
};
