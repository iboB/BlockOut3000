set(game BlockOut3000)
add_executable(${game}
    EntryPoint.cpp
)

target_link_libraries(${game}
    itlib
    yama
)
