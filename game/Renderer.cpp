// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Renderer.hpp"

#include "Vertex.hpp"

#include "lib/glsl-prefix.h"

#include <yama/matrix4x4.hpp>

#include <cstddef>

Renderer::Renderer() {}

Renderer::~Renderer()
{
    sg_destroy_pipeline(m_solidTrisPipeline);
    sg_destroy_shader(m_shader);
}

void Renderer::init()
{
    {
        const char* vert_src = GLSL_VERT_PREFIX
            R"glsl(
            uniform mat4 u_projView;
            in vec4 a_position;
            void main() {
                gl_Position = u_projView * a_position;
            }
            )glsl";

        const char* frag_src = GLSL_FRAG_PREFIX
            R"glsl(
            uniform vec4 u_color;
            out vec4 frag_color;
            void main() {
                frag_color = u_color;
            }
            )glsl";

        sg_shader_desc desc = {};

        auto& vub = desc.vs.uniform_blocks[0];
        vub.size = sizeof(yama::matrix);
        vub.uniforms[0].name = "u_projView";
        vub.uniforms[0].type = SG_UNIFORMTYPE_MAT4;

        auto& fub = desc.fs.uniform_blocks[0];
        fub.size = sizeof(yama::vector4);
        fub.uniforms[0].name = "u_color";
        fub.uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;

        desc.attrs[0].name = "a_position";

        desc.vs.source = vert_src;
        desc.fs.source = frag_src;
        m_shader = sg_make_shader(&desc);
    }

    {
        sg_pipeline_desc desc = {};
        desc.layout.buffers[0].stride = sizeof(Vertex);

        auto& attrs = desc.layout.attrs;
        attrs[0].offset = offsetof(Vertex, pos);
        attrs[0].format = SG_VERTEXFORMAT_FLOAT3;

        desc.shader = m_shader;

        desc.index_type = SG_INDEXTYPE_NONE;
        desc.face_winding = SG_FACEWINDING_CCW;

        desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
        desc.depth.write_enabled = true;

        ////////////////////////////////////////

        desc.primitive_type = SG_PRIMITIVETYPE_LINES;
        m_wirePipeline = sg_make_pipeline(desc);

        ////////////////////////////////////////

        desc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
        desc.cull_mode = SG_CULLMODE_BACK;
        m_solidTrisPipeline = sg_make_pipeline(desc);

        ////////////////////////////////////////

        desc.depth.write_enabled = false;
        auto& blend = desc.colors->blend;
        blend.enabled = true;
        blend.src_factor_rgb = blend.src_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
        blend.dst_factor_rgb = blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

        m_alphaFrontPipeline = sg_make_pipeline(desc);

        ////////////////////////////////////////
        desc.cull_mode = SG_CULLMODE_FRONT;

        m_alphaBackPipeline = sg_make_pipeline(desc);
    }
}
