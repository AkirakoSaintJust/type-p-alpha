cmake_minimum_required(VERSION 3.22)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 9.0.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

option(P1_BUILD_CLAP "Build the CLAP target" ON)
if(P1_BUILD_CLAP)
    FetchContent_Declare(
        clap-juce-extensions
        GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
        GIT_TAG 6bde108329ff9fc24404b79b8b397261a75cf6a8
        GIT_SUBMODULES "clap-libs/clap" "clap-libs/clap-helpers"
    )
    FetchContent_MakeAvailable(clap-juce-extensions)
endif()

set(P1_TOKENS_HEADER "${P1_GENERATED_DIR}/UiTokens.h")

add_custom_command(
    OUTPUT "${P1_TOKENS_HEADER}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${P1_GENERATED_DIR}"
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../tools/gen_tokens.py"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../design/tokens.json" "${P1_TOKENS_HEADER}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../../design/tokens.json"
            "${CMAKE_CURRENT_SOURCE_DIR}/../tools/gen_tokens.py"
    VERBATIM
)
add_custom_target(p1_tokens DEPENDS "${P1_TOKENS_HEADER}")

juce_add_binary_data(p1_resources SOURCES
    resources/fonts/DotGothic16-Regular.ttf
    resources/fonts/ZenDots-Regular.ttf
    resources/fonts/Unbounded-Regular.ttf
)

set(P1_JUCE_DEFINITIONS
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_JACK=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

set(P1_UI_SOURCES
    PluginEditor.cpp
    Pages.cpp
    PagesOperators.cpp
    PagesFxMod.cpp
    PagesFeedback.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/Theme.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/Widgets.cpp
)

add_library(p1_editor STATIC ${P1_UI_SOURCES})
set_target_properties(p1_editor PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(p1_editor PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../ui"
    "${P1_GENERATED_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(p1_editor PUBLIC
    p1_engine
    p1_resources
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
target_compile_definitions(p1_editor PUBLIC ${P1_JUCE_DEFINITIONS})
add_dependencies(p1_editor p1_params p1_tokens)

if(NOT P1_SANITIZERS)
juce_add_plugin(${P1_PLUGIN_TARGET}
    COMPANY_NAME "P1Onken"
    BUNDLE_ID "${P1_BUNDLE_ID}"
    PLUGIN_MANUFACTURER_CODE ${P1_MANUFACTURER_CODE}
    PLUGIN_CODE ${P1_PLUGIN_CODE}
    PRODUCT_NAME "${P1_PRODUCT_NAME}"
    FORMATS VST3 Standalone
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(${P1_PLUGIN_TARGET} PRIVATE PluginProcessor.cpp)
target_link_libraries(${P1_PLUGIN_TARGET} PRIVATE
    p1_editor
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
target_compile_definitions(${P1_PLUGIN_TARGET} PRIVATE ${P1_JUCE_DEFINITIONS})
add_dependencies(${P1_PLUGIN_TARGET} p1_params p1_tokens)

if(P1_BUILD_CLAP)
    clap_juce_extensions_plugin(
        TARGET ${P1_PLUGIN_TARGET}
        CLAP_ID "${P1_BUNDLE_ID}"
        CLAP_FEATURES instrument
    )
    target_compile_definitions(${P1_PLUGIN_TARGET}_CLAP PRIVATE ${P1_JUCE_DEFINITIONS})
endif()
endif()

juce_add_console_app(p1_smoke PRODUCT_NAME "p1_smoke")
target_sources(p1_smoke PRIVATE PluginProcessor.cpp ../tests/plugin_smoke.cpp)
target_link_libraries(p1_smoke PRIVATE
    p1_editor
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
target_compile_definitions(p1_smoke PRIVATE ${P1_JUCE_DEFINITIONS})
add_dependencies(p1_smoke p1_params p1_tokens)
add_test(NAME plugin_smoke COMMAND p1_smoke)

juce_add_console_app(p1_stability PRODUCT_NAME "p1_stability")
target_sources(p1_stability PRIVATE PluginProcessor.cpp ../tests/plugin_stability.cpp)
target_link_libraries(p1_stability PRIVATE
    p1_editor
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
target_compile_definitions(p1_stability PRIVATE ${P1_JUCE_DEFINITIONS})
add_dependencies(p1_stability p1_params p1_tokens)
add_test(NAME plugin_stability COMMAND p1_stability)

juce_add_console_app(p1_editor_snapshot PRODUCT_NAME "p1_editor_snapshot")
target_sources(p1_editor_snapshot PRIVATE PluginProcessor.cpp
                                        ../tests/editor_snapshot.cpp)
target_link_libraries(p1_editor_snapshot PRIVATE
    p1_editor
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
target_compile_definitions(p1_editor_snapshot PRIVATE ${P1_JUCE_DEFINITIONS})
add_dependencies(p1_editor_snapshot p1_params p1_tokens)

find_program(XVFB_RUN xvfb-run)
if(XVFB_RUN)
    add_test(NAME plugin_editor_snapshot
             COMMAND ${XVFB_RUN} -a "$<TARGET_FILE:p1_editor_snapshot>"
                     "${CMAKE_CURRENT_BINARY_DIR}/snapshots")
else()
    add_test(NAME plugin_editor_snapshot
             COMMAND "$<TARGET_FILE:p1_editor_snapshot>"
                     "${CMAKE_CURRENT_BINARY_DIR}/snapshots")
endif()

# The golden set is pinned to the Linux container because font rasterisation
# differs on other hosts. On Windows the snapshot binary still writes images,
# but the gate is not registered.
if(UNIX AND NOT APPLE)
    add_test(NAME plugin_editor_snapshot_diff
             COMMAND ${Python3_EXECUTABLE}
                     "${CMAKE_CURRENT_SOURCE_DIR}/../tools/snapshot_check.py"
                     --actual "${CMAKE_CURRENT_BINARY_DIR}/snapshots"
                     --golden "${CMAKE_CURRENT_SOURCE_DIR}/../tests/snapshots/linux-x86_64"
                     --diff "${CMAKE_CURRENT_BINARY_DIR}/snapshot-diff")
    set_tests_properties(plugin_editor_snapshot_diff PROPERTIES
                         DEPENDS plugin_editor_snapshot
                         TIMEOUT 120)
endif()

find_program(ARDOUR8_EXPORT ardour8-export)
find_program(CARLA_JACK_SINGLE carla-jack-single HINTS /opt/carla/bin)
find_program(JACK_REC jack_rec)
if(UNIX AND NOT APPLE AND NOT P1_SANITIZERS AND XVFB_RUN AND ARDOUR8_EXPORT
   AND CARLA_JACK_SINGLE AND JACK_REC)
    set(P1_ARTEFACTS
        "${CMAKE_CURRENT_BINARY_DIR}/${P1_PLUGIN_TARGET}_artefacts/$<CONFIG>")
    set(P1_LINUX_DAWS_CLAP)
    if(P1_BUILD_CLAP)
        set(P1_LINUX_DAWS_CLAP
            "${P1_ARTEFACTS}/CLAP/${P1_PRODUCT_NAME}.clap")
    endif()
    add_test(NAME linux_daws
             COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/../tests/linux_daws.sh"
                     "${P1_ARTEFACTS}/VST3/${P1_PRODUCT_NAME}.vst3"
                     "${CMAKE_CURRENT_BINARY_DIR}/linux-daws"
                     ${P1_LINUX_DAWS_CLAP})
    set_tests_properties(linux_daws PROPERTIES TIMEOUT 300)
endif()
