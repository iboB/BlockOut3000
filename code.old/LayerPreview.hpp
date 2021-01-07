// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// Preview of current stacked layers

#include <Magnum/GL/Mesh.h>

class LayerPreview
{
public:
    LayerPreview();
    ~LayerPreview();

    void draw(size_t numLayers) const;

private:
    // physical data
    mutable Magnum::GL::Mesh m_wire;
    mutable Magnum::GL::Mesh m_solid;

};
