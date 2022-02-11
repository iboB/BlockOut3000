CPMAddPackage(
    NAME sokol
    GITHUB_REPOSITORY floooh/sokol
    GIT_TAG 173f73e27c70a9ef91443aa292220cdad76493b1
)

add_library(sokol INTERFACE)
target_include_directories(sokol INTERFACE ${sokol_SOURCE_DIR})

if(UNIX)
    find_package(Threads)
    target_link_libraries(sokol INTERFACE
        X11 Xi Xcursor GL dl
        ${CMAKE_THREAD_LIBS_INIT}
    )
endif()
