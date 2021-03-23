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

    sg_pipeline solidTrisPipeline() const { return m_solidTrisPipeline; }
    sg_pipeline wirePipeline() const { return m_wirePipeline; }
    sg_pipeline alphaFrontPipeline() const { return m_alphaFrontPipeline; }
    sg_pipeline alphaBackPipeline() const { return m_alphaBackPipeline; }

private:
    sg_shader m_shader = {};

    sg_pipeline m_solidTrisPipeline;
    sg_pipeline m_wirePipeline;
    sg_pipeline m_alphaFrontPipeline;
    sg_pipeline m_alphaBackPipeline;
};
