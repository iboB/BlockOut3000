#include "Renderer.hpp"

#include "lib/glsl-prefix.h"

#include <yama/matrix4x4.hpp>

Renderer::Renderer() {

}

Renderer::~Renderer() {
    sg_destroy_shader(m_shader);
}

void Renderer::init() {
    {
        const char* vert_src =
            GLSL_VERT_PREFIX
            R"glsl(
            uniform mat4 u_projView;
            in vec4 a_position;
            void main() {
                gl_Position = u_projView * a_position;
            }
            )glsl";

        const char* frag_src =
            GLSL_FRAG_PREFIX
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

        auto& fub = desc.vs.uniform_blocks[0];
        fub.size = sizeof(yama::vector4);
        fub.uniforms[0].name = "u_color";
        fub.uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;

        desc.attrs[0].name = "a_position";

        desc.vs.source = vert_src;
        desc.fs.source = frag_src;
        m_shader = sg_make_shader(&desc);
    }
}
