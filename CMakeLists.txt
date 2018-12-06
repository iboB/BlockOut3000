cmake_minimum_required(VERSION 3.2.2)

project(BlockOut3000)

option(WITH_LOCAL_MAGNUM "Use a local clone of magnum" OFF)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
)

include(src_group)

if(${WITH_LOCAL_MAGNUM})
    add_subdirectory(corrade)
    add_subdirectory(magnum)
endif()

add_subdirectory(code)
