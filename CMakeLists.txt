cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(BlockOut3000)

# cpm
include(./cpm/get_cpm.cmake)
macro(dep packageName)
    include(./cpm/${packageName}.cmake)
endmacro()

# cmake lib
dep(icm)
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${icm_SOURCE_DIR}
)

# compiler settings
if(MSVC)
    set(additionalCompilerFlags "-D_CRT_SECURE_NO_WARNINGS=1")
else()
    set(additionalCompilerFlags "-Wall -Wextra")
endif()

option(SAN_ADDR "M: Sanitize address" OFF)

if(SAN_ADDR)
    if(MSVC)
        # set(sanitizerFlags "-fsanitize=address")
    elseif(APPLE)
        # apple clang doesn't support the leak sanitizer
        set(sanitizerFlags "-fsanitize=address,undefined -pthread -g")
    else()
        set(sanitizerFlags "-fsanitize=address,undefined,leak -pthread -g")
    endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${additionalCompilerFlags} ${sanitizerFlags}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${additionalCompilerFlags} ${sanitizerFlags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${sanitizerFlags}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${sanitizerFlags}")

# other
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # all executables go to bin
# enable_testing()

# packages
dep(itlib)
dep(yama)
dep(imgui)
dep(sokol)
dep(rlayout)

# subdirs
add_subdirectory(game)
