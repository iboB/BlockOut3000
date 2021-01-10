set(game BlockOut3000)
add_executable(${game}
    # sokol and imgui
    imgui.hpp
    sokol-app.h
    sokol-gfx.h
    sokol-impl.h
    sokol-impl.c
    sokol-imgui.hpp
    sokol-imgui.cpp

    # app
    App.hpp
    App.cpp
    AppModePtr.hpp
    AppMode.hpp
    AppMode.cpp

    # game
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
