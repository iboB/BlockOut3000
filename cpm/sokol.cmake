CPMAddPackage(
    NAME sokol
    GITHUB_REPOSITORY floooh/sokol
    GIT_TAG 21c9d47c85324104b0a5eeae3e017bd3551df7bb
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
