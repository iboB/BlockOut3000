set(game BlockOut3000)
add_executable(${game}
    # sokol and imgui
    lib/imgui.hpp
    lib/sokol-app.h
    lib/sokol-gfx.h
    lib/sokol-impl.h
    lib/sokol-impl.c
    lib/sokol-imgui.hpp
    lib/sokol-imgui.cpp
    lib/glsl-prefix.h

    # app
    App.hpp
    App.cpp
    AppModePtr.hpp
    AppMode.hpp
    AppMode.cpp
    Renderer.hpp
    Renderer.cpp

    # game
    BlockPrimitives.hpp
    BlockBuilder.hpp
    BlockBuilder.cpp
    BlockTemplate.hpp
    BlockTemplate.cpp

    # modes
    ModeFatalError.hpp
    ModeFatalError.cpp
    ModeExperimental.hpp
    ModeExperimental.cpp
)

target_link_libraries(${game}
    itlib
    yama
    sokol
    imgui
)
