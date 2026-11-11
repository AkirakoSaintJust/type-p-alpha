cmake_minimum_required(VERSION 3.22)
project(type_p_alpha VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(P1_BUILD_JUCE ON CACHE BOOL "" FORCE)
set(P1_BUILD_CLAP ON CACHE BOOL "" FORCE)
set(P1_SANITIZERS "" CACHE STRING
    "clang sanitizers, for example address or address,undefined")
if(P1_SANITIZERS)
    add_compile_options(-fsanitize=${P1_SANITIZERS} -fno-omit-frame-pointer)
    add_link_options(-fsanitize=${P1_SANITIZERS})
endif()
enable_testing()
add_subdirectory(plugin)
