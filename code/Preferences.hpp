// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <Magnum/Magnum.h>

// class that stores preferences, that the user may edit

struct Preferences
{
public:
    // time for the figure to fall a unit when dropped
    static int figureDropTime();

    static const Magnum::Color4& layerColor(uint32_t layer);
};