// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#if !defined(NDEBUG)
#    define SOKOL_DEBUG 1
#endif

#if defined(__EMSCRIPTEN__)
#    define SOKOL_GLES3
#else
#    define SOKOL_GLCORE33
#endif
