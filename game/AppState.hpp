// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once
#include "AppStatePtr.hpp"

#include "lib/Time.hpp"
#include "lib/vec2.hpp"

extern "C" struct sapp_event;

class AppState
{
public:
    virtual ~AppState();

    // for debugging purposes
    virtual const char* name() const = 0;

    // shows that the state has completed its purpose
    // and can be released by the application
    // and substituted with a pending state
    bool completed() const { return m_completed; }

    // app flow
    virtual bool activate();
    virtual void update(ms_t dt, ivec2 windowSize);
    virtual void defaultRender(ivec2 windowSize);
    virtual bool handleEvent(const sapp_event& event);
    virtual void deactivate();

    // optionally override to provide a next state when states are changed
    // may be called after deactivate!
    virtual AppStatePtr getNextState();

protected:
    void setComplete() { m_completed = true; }
    bool m_completed = false;
};
