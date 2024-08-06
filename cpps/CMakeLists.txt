cmake_minimum_required(VERSION 3.10)
project(TinySeries)

option(WASM "Build for WebAssembly" OFF)

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WASM)
    if(NOT DEFINED ENV{EMSCRIPTEN_ROOT})
        find_path(EMSCRIPTEN_ROOT "emcc")
    else()
        set(EMSCRIPTEN_ROOT "$ENV{EMSCRIPTEN_ROOT}")
    endif()

    if(NOT EMSCRIPTEN_ROOT)
        if(NOT DEFINED ENV{EMSDK})
            message(FATAL_ERROR "The emcc compiler not found in PATH")
        endif()
        set(EMSCRIPTEN_ROOT "$ENV{EMSDK}/upstream/emscripten")
    endif()

    if(EXISTS "${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake")
        else()
        message(FATAL_ERROR "Emscripten.cmake toolchain file not found")
    endif()
endif()

include_directories(common third-party)
file(GLOB_RECURSE COMMON_SOURCES common/*.cpp common/*.h third-party/*.cpp third-parth/*.h)

# Add subdirectories for each project
add_subdirectory(TinyRenderer)
add_subdirectory(TinyRaytracer)
