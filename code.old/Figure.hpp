// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Magnum.h>

#include <vector>

// figure class

class FigureTemplate;
class Pit;

class Figure
{
public:
    Figure(const FigureTemplate& tmpl, Pit& pit);

    void update(uint32_t dt);

    void draw();

    // call for the figure to set itself at a starting position in the pit
    // return false if it cannot
    bool spawn();

    // all functions are "try" because they wont perform the required transform
    // unless it's possible on the pit
    // they return false if the transform was unsuccessful

    // parameter is -1/1 for cw/ccw rotation
    bool tryRotateX(float dir);
    bool tryRotateY(float dir);
    bool tryRotateZ(float dir);

    // axis is 0=x, 1=y, 2=z
    bool tryRotate(int axis, float dir, bool animate = true, bool force = false);

    bool tryMove(const Magnum::Vector3& dir, bool animate = true, bool force = false);

    bool isFallen() const { return m_isFallen; }

    // figure will try to fall faster
    void startDrop();
    void stopDrop();

private:

    // check tryElems with pit
    bool checkTryElementsWithPit() const;

    // gives the fall time for a given speed
    // if the figure dropped, it ignores the speed and just returns the drop fall time
    int fallTimeForSpeed(int speed) const;

    //////////////////////////////////////////////////
    // logical data
    // gameplay data about the figure
    // after every transformation this is recalculated
    // so that every transformation is always absolute

    const FigureTemplate& m_template;
    Pit& m_pit;

    std::vector<Magnum::Vector3i> m_elements;

    // used when a transformation is performed to check if it's possible on the pit
    std::vector<Magnum::Vector3i> m_tryElements;

    // is figure fallen?
    bool m_isFallen = false;

    // is figure falling faster (user wants it to fall)
    bool m_isDropped = false;

    // fall timer
    int m_fallTimer;

    //////////////////////////////////////////////////
    // physical data
    // used to draw the figure mesh

    // model transformation matrix for drawing the figure
    // it is calculated every frame
    Magnum::Matrix4 m_transform;

    // current means including animations
    // m_transform is calculated from these
    Magnum::Vector3 m_currentPosition;
    Magnum::Quaternion m_currentRotation;

    // endpoints for animation
    Magnum::Vector3 m_lastPosition;
    Magnum::Vector3 m_targetPosition;

    Magnum::Quaternion m_lastRotation;
    Magnum::Quaternion m_targetRotation;

    // timers for animation, when they reach 0, current = target
    int m_positionAnimationTimer;
    int m_rotationAnimationTimer;
};