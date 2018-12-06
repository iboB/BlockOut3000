// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Preferences.hpp"

#include <Magnum/Math/Color.h>

#include <vector>

using namespace std;
using namespace Magnum;

namespace
{
vector<Color4> m_levelLayerColors =
{
    {0, 0, 0.8f, 1},
    {0, 0.85f, 0, 1},
    {0, 0.6f, 0.6f, 1},
    {0.75f, 0, 0, 1},
    {0.70f, 0.1f, 0.55f, 1},
    {0.88f, 0.80f, 0.1f, 1},
    {0.40f, 0.0f, 0.88f, 1},
    {0.98f, 0.5f, 0.0f, 1},
    {0.98f, 0.88f, 0.88f, 1},
    {0.58f, 0.48f, 0.58f, 1},
    {0, 0, 0.8f, 1},
    {0, 0.85f, 0, 1},
    {0, 0.6f, 0.6f, 1},
    {0.75f, 0, 0, 1},
    {0.70f, 0.1f, 0.55f, 1},
    {0.88f, 0.80f, 0.1f, 1},
    {0.40f, 0.0f, 0.88f, 1},
    {0.98f, 0.5f, 0.0f, 1},
    {0.98f, 0.88f, 0.88f, 1},
    {0.58f, 0.48f, 0.58f, 1},
};
}

int Preferences::figureDropTime()
{
    return 100;
}

const Color4& Preferences::layerColor(uint32_t layer)
{
    static const Color4 defaultColor = { 0.5, 0.5, 0.5, 1 };
    if (layer >= m_levelLayerColors.size())
    {
        return defaultColor;
    }

    return m_levelLayerColors[layer];
}