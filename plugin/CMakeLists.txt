cmake_minimum_required(VERSION 3.22)
project(p1onken_plugin_host VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    enable_language(OBJCXX)
endif()

option(P1_BUILD_JUCE "Build the JUCE VST3 and standalone host" OFF)

# The checked-in Type P.alpha engine and product identity for a plain
# `cmake -S plugin` build or the root `cmake -S .` build.
set(P1_ENGINE "${CMAKE_CURRENT_SOURCE_DIR}/../engine" CACHE PATH "engine root")
set(P1_SYNTH_GEN "${P1_ENGINE}/gen")
set(P1_SYNTH_CNAME "type_p_alpha" CACHE STRING "the synth's generated C identifier")
set(P1_PRODUCT_NAME "Type P.alpha" CACHE STRING "the plugin display name")
set(P1_PLUGIN_TARGET "${P1_SYNTH_CNAME}" CACHE STRING "the CMake plugin target")
set(P1_BUNDLE_ID "com.p1onken.typepalpha" CACHE STRING "the JUCE bundle id")
set(P1_MANUFACTURER_CODE "P1on" CACHE STRING "the JUCE manufacturer code")
set(P1_PLUGIN_CODE "Tpal" CACHE STRING "the JUCE plugin code")
set(P1_STATE_TREE_ID "${P1_SYNTH_CNAME}" CACHE STRING "the APVTS state tree id")

find_package(Python3 3.8 REQUIRED COMPONENTS Interpreter)
set(P1_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${P1_GENERATED_DIR}")

# The engine bridge and the editor talk to this alias header, not to the synth's
# generated names, so the host is reusable.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/P1Synth.h.in"
    "${P1_GENERATED_DIR}/P1Synth.h"
    @ONLY
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/P1Product.h.in"
    "${P1_GENERATED_DIR}/P1Product.h"
    @ONLY
)

set(P1_CAPABILITIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/capabilities"
    CACHE PATH "the plugin-only capability packages")
set(P1_CAPABILITIES_ENABLED "${P1_CAPABILITIES_DIR}/enabled.json"
    CACHE FILEPATH "the enabled capability list")
set(P1_PLUGIN_PARAMS "${P1_GENERATED_DIR}/plugin-params.json")
execute_process(
    COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_capabilities.py"
            --params "${P1_SYNTH_GEN}/params.json"
            --dir "${P1_CAPABILITIES_DIR}"
            --enabled "${P1_CAPABILITIES_ENABLED}"
            --out "${P1_GENERATED_DIR}"
    RESULT_VARIABLE P1_CAPABILITY_RESULT
    OUTPUT_VARIABLE P1_CAPABILITY_OUTPUT
    ERROR_VARIABLE P1_CAPABILITY_ERROR
)
if(P1_CAPABILITY_RESULT)
    message(FATAL_ERROR "capabilities: ${P1_CAPABILITY_ERROR}")
endif()
message(STATUS "${P1_CAPABILITY_OUTPUT}")
include("${P1_GENERATED_DIR}/CapabilitySources.cmake")
file(GLOB_RECURSE P1_CAPABILITY_FILES CONFIGURE_DEPENDS
    "${P1_CAPABILITIES_DIR}/*")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_capabilities.py")

set(P1_PARAMS_HEADER "${P1_GENERATED_DIR}/Params.h")
add_custom_command(
    OUTPUT "${P1_PARAMS_HEADER}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${P1_GENERATED_DIR}"
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_params.py"
            "${P1_PLUGIN_PARAMS}" "${P1_PARAMS_HEADER}"
    DEPENDS "${P1_PLUGIN_PARAMS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_params.py"
    VERBATIM
)
add_custom_target(p1_params DEPENDS "${P1_PARAMS_HEADER}")

if(MSVC OR (WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(P1_MSVC_ABI ON)
else()
    set(P1_MSVC_ABI OFF)
endif()

if(P1_MSVC_ABI)
    set(UMETATE_STATE_PTR "__declspec(thread)")
else()
    set(UMETATE_STATE_PTR "_Thread_local")
endif()

enable_testing()

add_library(p1_engine STATIC
    engine/Engine.cpp
    engine/MidiBridge.cpp
    engine/RateBridge.cpp
    preset/PresetStore.cpp
    preset/presets_host.c
    "${P1_SYNTH_GEN}/${P1_SYNTH_CNAME}_dsp.c"
    "${P1_SYNTH_GEN}/tables.c"
    "${P1_ENGINE}/runtime/synth_midi.c"
    "${P1_CAPABILITY_DSP_SOURCE}"
)
set_target_properties(p1_engine PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(p1_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/preset
    ${P1_SYNTH_GEN}
    ${P1_GENERATED_DIR}
    ${P1_ENGINE}/gen
    ${P1_ENGINE}/hal
    ${P1_ENGINE}/runtime
    ${P1_CAPABILITY_INCLUDE_DIRS}
)

target_compile_definitions(p1_engine PRIVATE
    UMETATE_MIDI_EXTERNAL
    "UMETATE_STATE_PTR=${UMETATE_STATE_PTR}"
)

if(P1_MSVC_ABI)
    target_compile_options(p1_engine PRIVATE
        "/FI${CMAKE_CURRENT_SOURCE_DIR}/engine/msvc_compat.h")
endif()

add_executable(engine_parity tests/engine_parity.cpp)
target_link_libraries(engine_parity PRIVATE p1_engine)
target_include_directories(engine_parity PRIVATE "${P1_GENERATED_DIR}")
add_dependencies(engine_parity p1_params)
add_test(NAME engine_parity COMMAND engine_parity)

add_executable(rate_bridge_test tests/rate_bridge_test.cpp)
target_link_libraries(rate_bridge_test PRIVATE p1_engine)
add_test(NAME rate_bridge COMMAND rate_bridge_test)

add_executable(preset_smoke tests/preset_smoke.cpp)
target_link_libraries(preset_smoke PRIVATE p1_engine)
target_include_directories(preset_smoke PRIVATE "${P1_GENERATED_DIR}")
add_dependencies(preset_smoke p1_params)
add_test(NAME preset_smoke COMMAND preset_smoke)

if(P1_BUILD_JUCE)
    add_subdirectory(juce)
endif()
