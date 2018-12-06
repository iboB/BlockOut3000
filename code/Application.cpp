// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include <Magnum/GL/Context.h>
#include <Magnum/Timeline.h>

#include "AppState.hpp"

#include "State_Test.hpp"
#include "State_InGame.hpp"

#include "Application_State.hpp"

#include "Resources.hpp"

using namespace Magnum;

namespace
{

class App;
App* instance;

class App : public Platform::Application
{
public:
    explicit App(const Arguments& arguments)
        : Platform::Application{ arguments, Configuration{}.setTitle("Tris") }
    {
        instance = this;

        using namespace Magnum::Math::Literals;

        setSwapInterval(1);
        setMinimalLoopPeriod(16);

        Resources::initialize();

        // state
        //setState(State_Test());
        setState(State_InGame());

        // time
        m_timeline.start();
    }

    ~App()
    {
        instance = nullptr;

        if (m_nextState)
        {
            m_nextState->cleanup();
        }
        m_currentState->cleanup();

        Resources::cleanup();
    }

    void keyPressEvent(KeyEvent& event) override
    {
        m_currentState->keyPressEvent(event);
    }

    void keyReleaseEvent(KeyEvent& event) override
    {
        m_currentState->keyReleaseEvent(event);
    }

    void mousePressEvent(MouseEvent&) override
    {
    }

    void mouseMoveEvent(MouseMoveEvent&) override
    {
    }

    void mouseScrollEvent(MouseScrollEvent&) override
    {
    }

    void multiGestureEvent(MultiGestureEvent&) override
    {
    }

    void tickEvent() override
    {
        checkForStateChange();
        auto dt = uint32_t(m_timeline.previousFrameDuration() * 1000);
        m_currentState->update(dt);
    }

    void drawEvent() override
    {
        GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

        m_currentState->draw();

        swapBuffers();
        m_timeline.nextFrame();
        redraw();
    }

    void checkForStateChange()
    {
        if (!m_nextState) return;

        auto next = m_nextState.release(); // in case a failed initialize sets another next state
        if (!next->initialize())
        {
            Error{} << "Error initializing" << next->name() << "Aborting\n";
            delete next;
        }
        else
        {
            Debug{} << "Switching to state" << next->name();;
            m_currentState->cleanup();
            m_currentState.reset(next);
        }
    }

    void setState(std::unique_ptr<AppState> state)
    {
        if (m_nextState)
        {
            // we have a pending but never realized next state
            // abort and forget

            Warning{} << "Ignoring state " << m_nextState->name() << " when switching to " << state->name();
            m_nextState.reset();
        }

        m_nextState.reset(state.release());
    }

    Timeline m_timeline;

    // states
    std::unique_ptr<AppState> m_currentState = State_Noop();
    std::unique_ptr<AppState> m_nextState;
};

}

namespace Application
{

void setState(std::unique_ptr<AppState> state)
{
    instance->setState(std::move(state));
}

}

MAGNUM_APPLICATION_MAIN(App)
