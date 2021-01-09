#pragma once

#if !defined(NDEBUG)
#    define SOKOL_DEBUG 1
#endif

#if defined(__EMSCRIPTEN__)
#    define SOKOL_GLES3
#else
#    define SOKOL_GLCORE33
#endif
