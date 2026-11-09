cmake_minimum_required(VERSION 3.22)
project(type_p_alpha VERSION 0.1.0 LANGUAGES C CXX)

set(P1_BUILD_JUCE ON CACHE BOOL "" FORCE)
set(P1_BUILD_CLAP ON CACHE BOOL "" FORCE)
enable_testing()
add_subdirectory(plugin)
