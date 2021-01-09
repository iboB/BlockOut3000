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

    # game
    App.hpp
    App.cpp
)

target_link_libraries(${game}
    itlib
    yama
    sokol
    imgui
)
