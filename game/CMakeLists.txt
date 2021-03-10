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
    lib/matrix.hpp
    lib/Mesh.hpp
    lib/Mesh.cpp

    # app
    App.hpp
    App.cpp
    AppModePtr.hpp
    AppMode.hpp
    AppMode.cpp
    Renderer.hpp
    Renderer.cpp

    # game
    Cube.hpp
    Cube.cpp
    # BlockPrimitives.hpp
    BlockBuilder.hpp
    BlockBuilder.cpp
    BlockTemplate.hpp
    BlockTemplate.cpp
    Pit.hpp
    Pit.cpp

    # modes
    ModeFatalError.hpp
    ModeFatalError.cpp
    ModeExperimental.hpp
    ModeExperimental.cpp
 "lib/sokol-gfx-utils.hpp")

target_link_libraries(${game}
    itlib
    yama
    sokol
    imgui
)
