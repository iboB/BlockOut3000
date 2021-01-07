// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "State_InGame.hpp"
#include "AppState.hpp"

#include "Pit.hpp"
#include "Figure.hpp"
#include "FigureSet.hpp"
#include "FigureManager.hpp"
#include "Random.hpp"
#include "Resources.hpp"
#include "Flat3DPVM.hpp"
#include "LayerPreview.hpp"

#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <ctime>
#include <cassert>

using namespace Magnum;

namespace
{

// default implementations
class InGameState : public AppState
{
    const char* name() const { return "ingame"; }
    bool initialize() override
    {
        Random::seed(time(nullptr));

        GL::Renderer::setClearColor({ 0.05f, 0.05f, 0.05f });
        GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
        GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

        FigureManager::initialize();
        if (!FigureManager::readFigures(Resources::figuresData()))
            return false;

        m_pit.reset(new Pit({ 5, 5, 10 }));

        m_figureSet = FigureManager::getFigureSet("Extended");
        assert(m_figureSet);

        m_nextFigure.reset(new Figure(*m_figureSet->getRandomFigureTemplate(), *m_pit));
        spawnNextFigure();

        m_layerPreview.reset(new LayerPreview);

        return true;
    }

    void cleanup() override
    {
        m_layerPreview.reset();
        m_currentFigure.reset();
        m_nextFigure.reset();
        m_pit.reset();
        FigureManager::cleanup();
    }

    void update(uint32_t dt) override
    {
        m_currentFigure->update(dt);
        m_pit->update(dt);

        if (m_currentFigure->isFallen())
            spawnNextFigure();
    }

    void draw() override
    {
        // Draw main gameplay
        GL::defaultFramebuffer.setViewport(Range2Di::fromSize({ 80, 10 }, { 580, 580 }));

        m_pit->draw();
        m_currentFigure->draw();

        auto proj = Matrix4::perspectiveProjection(Rad{ Constants::piHalf() }, 1, 1, 100);
        auto view = Matrix4::lookAt({ 1, 1, 3.5f }, { 1, 1, 0 }, { 0, 1, 0 }).invertedRigid();

        auto pv = proj * view;
        Resources::shaderFlat3D().setProjView(pv);

        GL::defaultFramebuffer.setViewport(Range2Di::fromSize({ 670, 470 }, { 120, 120 }));

        m_nextFigure->draw();

        GL::defaultFramebuffer.setViewport(Range2Di::fromSize({ 10, 10 }, { 60, 580 }));

        m_layerPreview->draw(m_pit->topNonEmptyLayer());
    }

    void spawnNextFigure()
    {
        m_currentFigure.reset(m_nextFigure.release());
        m_nextFigure.reset(new Figure(*m_figureSet->getRandomFigureTemplate(), *m_pit));
        m_currentFigure->spawn();
    }

    void keyPressEvent(Magnum::Platform::Application::KeyEvent& event) override
    {
        if (event.isRepeated()) return;

        using Key = Platform::Application::KeyEvent::Key;
        switch (event.key())
        {
        case Key::Q:
            m_currentFigure->tryRotateX(1);
            break;
        case Key::A:
            m_currentFigure->tryRotateX(-1);
            break;
        case Key::W:
            m_currentFigure->tryRotateY(1);
            break;
        case Key::S:
            m_currentFigure->tryRotateY(-1);
            break;
        case Key::E:
            m_currentFigure->tryRotateZ(1);
            break;
        case Key::D:
            m_currentFigure->tryRotateZ(-1);
            break;
        case Key::Left:
            m_currentFigure->tryMove({-1, 0, 0});
            break;
        case Key::Right:
            m_currentFigure->tryMove({1, 0, 0});
            break;
        case Key::Up:
            m_currentFigure->tryMove({0, 1, 0});
            break;
        case Key::Down:
            m_currentFigure->tryMove({0, -1, 0});
            break;
        case Key::Space:
            m_currentFigure->startDrop();
            break;
        default:
            return;
        }

        event.setAccepted();
    }

    void keyReleaseEvent(Magnum::Platform::Application::KeyEvent& event) override
    {
        using Key = Platform::Application::KeyEvent::Key;
        if (event.key() == Key::Space)
        {
            m_currentFigure->stopDrop();
            event.setAccepted();
        }
    }

    std::unique_ptr<Pit> m_pit;
    const FigureSet* m_figureSet; // set that is currently being used
    std::unique_ptr<Figure> m_currentFigure; // figure that is currently falling in the level
    std::unique_ptr<Figure> m_nextFigure; // figure that will be next
    std::unique_ptr<LayerPreview> m_layerPreview;
};

}

std::unique_ptr<AppState> State_InGame()
{
    return std::make_unique<InGameState>();
}
