#include "Renderer.hpp"

#include "lib/glsl-prefix.h"

namespace
{
const char* VERT_SRC =
    GLSL_VERT_PREFIX
    R"glsl(
        uniform mat4 u_projView;
        in vec4 a_position;
        in vec4 a_color;
        out vec4 v_color;
        void main() {
            v_color = a_color;
            gl_Position = u_projView * a_position;
        }
    )glsl";

const char* FRAG_SRC =
    GLSL_FRAG_PREFIX
    R"glsl(
        in vec4 v_color;
        out vec4 frag_color;
        void main() {
            frag_color = v_color;
        }
    )glsl";
}

Renderer::Renderer() {

}

Renderer::~Renderer() {

}

bool Renderer::init() {

}
