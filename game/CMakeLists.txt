set(game BlockOut3000)
add_executable(${game}
    # sokol and imgui
    lib/imgui.hpp
    lib/sokol-app.h
    lib/sokol-gfx.h
    lib/sokol-gfx-utils.hpp
    lib/sokol-impl.h
    lib/sokol-impl.c
    lib/sokol-imgui.hpp
    lib/sokol-imgui.cpp
    lib/glsl-prefix.h
    lib/matrix.hpp
    lib/Mesh.hpp
    lib/Mesh.cpp
    "lib/GUILayout.hpp"
    "lib/GUILayout.cpp"
    "lib/rlayout.hpp"
    "lib/rlayout.cpp"

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
    Primitives.hpp
    BlockDefinition.hpp
    BlockDefinition.cpp
    BlockBuilder.hpp
    BlockBuilder.cpp
    BlockTemplate.hpp
    BlockTemplate.cpp
    BlockManager.hpp
    BlockManager.cpp
    Pit.hpp
    Pit.cpp

    # modes
    ModeFatalError.hpp
    ModeFatalError.cpp
    ModeExperimental.hpp
    ModeExperimental.cpp
    "ModeBlockBuilder.hpp"
    "ModeBlockBuilder.cpp"
    "LayoutExperimental.hpp"
    "LayoutExperimental.cpp"
    "ModeLayoutTest.hpp"
    "ModeLayoutTest.cpp"
 "LayoutBlockBuilder.hpp" "LayoutBlockBuilder.cpp" "lib/sokol-glue.h")

target_link_libraries(${game}
    itlib
    yama
    sokol
    imgui
    rlayout
)
