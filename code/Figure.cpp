// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Figure.hpp"
#include "Pit.hpp"

#include "FigureTemplate.hpp"

#include "Resources.hpp"
#include "Flat3DPVM.hpp"
#include "Preferences.hpp"
#include "Random.hpp"

#include <Magnum/Math/Functions.h>

#include <cassert>

using namespace Magnum;
using namespace std;

// ms animations last
const int ROTATION_ANIM_TIME = 200;
const int TRANSLATION_ANIM_TIME = 100;

const int FALL_TIME = 800; // milliseconds to fall a unit

Figure::Figure(const FigureTemplate& tmpl, Pit& pit)
    : m_template(tmpl)
    , m_pit(pit)
    , m_elements(m_template.elements())
    , m_tryElements(m_template.elements())
    , m_fallTimer(fallTimeForSpeed(pit.speed()))
    , m_targetPosition(m_currentPosition)
    , m_targetRotation(m_currentRotation)
    , m_positionAnimationTimer(0)
    , m_rotationAnimationTimer(0)
{
    //// add random transform
    //for (int i = 0; i < 5; ++i)
    //{
    //    float dirs[] = { 0, -1, 1 };

    //    auto dir = InGameRndU32() % 3;
    //    if (!dir)
    //        continue;

    //    tryRotate(InGameRndU32() % 3, dirs[dir], false, true);
    //}
}

namespace
{
    // matrix that rotates around a certain center
    Matrix4 CreateOffCenterRotationMatrix(const Quaternion& rot, const Vector3& center)
    {
        return Matrix4::translation(center)
            * Matrix4(rot.toMatrix())
            * Matrix4::translation(-center);
    }
}

void Figure::update(uint32_t dt)
{
    if (m_isFallen) return;
    if (m_fallTimer -= dt)
    {
        if (m_fallTimer <= 0)
        {
            m_fallTimer = fallTimeForSpeed(m_pit.speed()) + m_fallTimer;
            if (!tryMove({0, 0, -1}))
            {
                // if figgure cannot move down, it's fallen
                m_pit.adoptFigure(m_elements);
                m_isFallen = true;
                return;
            }
        }
    }

    if (m_positionAnimationTimer)
    {
        m_positionAnimationTimer -= dt;
        if (m_positionAnimationTimer < 0)
            m_positionAnimationTimer = 0;

        float f = 1 - float(m_positionAnimationTimer) / TRANSLATION_ANIM_TIME;
        m_currentPosition = Math::lerp(m_lastPosition, m_targetPosition, f);
    }

    if (m_rotationAnimationTimer)
    {
        m_rotationAnimationTimer -= dt;
        if (m_rotationAnimationTimer < 0)
            m_rotationAnimationTimer = 0;

        float f = 1 - float(m_rotationAnimationTimer) / ROTATION_ANIM_TIME;
        m_currentRotation = slerp(m_lastRotation, m_targetRotation, f);
    }

    m_transform = Matrix4::translation(m_currentPosition)
        * CreateOffCenterRotationMatrix(m_currentRotation, m_template.rotationCenter());
}

void Figure::draw()
{
    auto& shader = Resources::shaderFlat3D();
    shader.setModel(m_transform);

    m_template.draw({ 0.9f, 0.5f, 0.5f, 0.1f }, { 1, 1, 1, 1 });
}

bool Figure::tryRotateX(float dir)
{
    return tryRotate(0, dir);
}

bool Figure::tryRotateY(float dir)
{
    return tryRotate(1, dir);
}

bool Figure::tryRotateZ(float dir)
{
    return tryRotate(2, dir);
}

bool Figure::tryRotate(int axis, float dir, bool animate /*= true*/, bool force /*= false*/)
{
    assert(!m_isFallen);
    static Vector3 axes[] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };

    auto rotation = Quaternion::rotation(Rad(dir * Constants::piHalf()), axes[axis]);

    // absolute transform for this rotation
    // based on it we will check if the figure can rotate like that
    auto transform = CreateOffCenterRotationMatrix(rotation, m_template.rotationCenter());

    // this 0.5 translation is used in order to compensate for the discrepancy of the elements positions data
    // and the rotation center. The rotation center being based on cubes from 000 to 111 and the elem data
    // integer coordinates of cubes in a grid
    vector<Vector3> felems(m_elements.size());
    for (size_t i = 0; i < m_elements.size(); ++i)
    {
        auto& e = m_elements[i];
        felems[i] = Vector3(e) - m_targetPosition + Vector3(0.5f);
    }

    for (auto& e : felems)
    {
        e = transform.transformPoint(e);
        e += m_targetPosition - Vector3(0.5f);
    }

    for (size_t i = 0; i < m_elements.size(); ++i)
    {
        auto& e = felems[i];
        m_tryElements[i] = Vector3i(Math::round(e));
    }

    if (!force && !checkTryElementsWithPit())
    {
        // desired transform cannot fit with pit
        return false;
    }

    m_elements = m_tryElements;

    // update transforms for the mesh
    if (animate)
    {
        m_lastRotation = m_currentRotation;
        m_targetRotation = rotation * m_targetRotation;
        m_rotationAnimationTimer = ROTATION_ANIM_TIME;
    }
    else
    {
        m_targetRotation = rotation * m_targetRotation;
        m_currentRotation = m_targetRotation;
        m_rotationAnimationTimer = 0;
    }

    return true;
}

bool Figure::tryMove(const Vector3& d, bool animate /*= true*/, bool force /*= false*/)
{
    assert(!m_isFallen);
    for (size_t i = 0; i < m_elements.size(); ++i)
    {
        m_tryElements[i] = m_elements[i] + Vector3i(Math::round(d));
    }

    if (!force && !checkTryElementsWithPit())
    {
        // desired transform cannot fit with pit
        return false;
    }

    m_elements = m_tryElements;

    // update mesh transforms
    if (animate)
    {
        m_lastPosition = m_currentPosition;
        m_targetPosition += d;
        m_positionAnimationTimer = TRANSLATION_ANIM_TIME;
    }
    else
    {
        m_currentPosition = m_targetPosition += d;
        m_positionAnimationTimer = 0;
    }

    return true;
}

bool Figure::checkTryElementsWithPit() const
{
    assert(!m_isFallen);
    if (!m_pit.canFitFigure(m_tryElements))
    {
        return false;
    }

    return true;
}

bool Figure::spawn()
{
    // Find highest point of figure
    int top = 0;
    for (const auto& e : m_elements)
    {
        if (e.z() > top)
            top = e.z();
    }

    float targetZ = float(m_pit.size().z() - 1 - top);

    // Fit rotation center of figure to center of pit
    auto pitCenter = Vector2(m_pit.size().xy()) / 2.f;

    auto translation = pitCenter - m_template.rotationCenter().xy();

    return tryMove({ round(translation.x()), round(translation.y()), targetZ }, false);
}

void Figure::startDrop()
{
    tryMove({ 0, 0, -1 });
    m_fallTimer = Preferences::figureDropTime();
    m_isDropped = true;
}

void Figure::stopDrop()
{
    m_isDropped = false;
}

int Figure::fallTimeForSpeed(int speed) const
{
    if (m_isDropped)
    {
        return Preferences::figureDropTime();
    }

    int speedMultiplier = (FALL_TIME - TRANSLATION_ANIM_TIME) / Pit::MAX_SPEED;
    return TRANSLATION_ANIM_TIME + (Pit::MAX_SPEED - speed) * speedMultiplier;
}
