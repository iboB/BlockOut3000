CPMAddPackage(
    NAME sokol
    GITHUB_REPOSITORY floooh/sokol
    GIT_TAG edd83be7ba2f18eec9ca44497a2c4b3acaa95b50
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
