// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "AppState.hpp"

AppState::~AppState() = default;

namespace
{

// default implementations
class NoopState : public AppState
{
    const char* name() const { return "noop"; }
    bool initialize() override { return true; }
    void cleanup() override {}
    void update(uint32_t) override {}
    void draw() override {}
};

}

void AppState::keyPressEvent(Magnum::Platform::Application::KeyEvent&) {}
void AppState::keyReleaseEvent(Magnum::Platform::Application::KeyEvent&) {}

std::unique_ptr<AppState> State_Noop()
{
    return std::make_unique<NoopState>();
}
