// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/sokol-gfx.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void init();

private:
    sg_shader m_shader = {};
};