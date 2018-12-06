// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <memory>

#include <Magnum/Platform/Sdl2Application.h> // temp :|

class AppState
{
public:
    virtual ~AppState();

    virtual const char* name() const = 0;

    // return false if not successful
    virtual bool initialize() = 0;
    virtual void cleanup() = 0;

    // input - TODO: redesign
    virtual void keyPressEvent(Magnum::Platform::Application::KeyEvent& event);
    virtual void keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event);

    // dt is time delta in milliseconds since last frame
    virtual void update(uint32_t dt) = 0;

    virtual void draw() = 0;
};

std::unique_ptr<AppState> State_Noop();
