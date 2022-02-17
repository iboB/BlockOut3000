// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#if defined(__EMSCRIPTEN__)
#   define GLSL_VERT_PREFIX "#version 300 es\n"
#   define GLSL_FRAG_PREFIX "#version 300 es\n" \
                            "precision mediump float;\n"
#else
#   define GLSL_VERT_PREFIX "#version 330\n"
#   define GLSL_FRAG_PREFIX "#version 330\n"
#endif
