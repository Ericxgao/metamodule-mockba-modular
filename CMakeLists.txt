cmake_minimum_required(VERSION 3.24)

if (DEFINED ENV{METAMODULE_SDK_DIR})
    set(METAMODULE_SDK_DIR "$ENV{METAMODULE_SDK_DIR}")
elseif("${METAMODULE_SDK_DIR}" STREQUAL "")
    set(METAMODULE_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../metamodule-plugin-sdk")
endif()
include(${METAMODULE_SDK_DIR}/plugin.cmake)

project(MockbaModular VERSION 2.0.1 DESCRIPTION "Mockba Modular Pack" LANGUAGES C CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MockbaModular)
set(DSP_DIR ${SOURCE_DIR}/dsp)

add_library(MockbaModular STATIC)

target_compile_definitions(MockbaModular PRIVATE 
    __V1x 
    __PLUGIN
)

# Add compile options for better control over symbols
target_compile_options(MockbaModular PRIVATE
    -fdata-sections
    -ffunction-sections
    -fno-exceptions
    -fno-threadsafe-statics
    -fno-use-cxa-atexit
    $<$<CXX_COMPILER_ID:GNU>:-fvisibility=hidden>
    $<$<CXX_COMPILER_ID:GNU>:-fvisibility-inlines-hidden>
)

# Add linker flags for static linking
target_link_options(MockbaModular PRIVATE
    -Wl,--gc-sections
    -static-libgcc
    -static-libstdc++
)

# Add source files
target_sources(MockbaModular PRIVATE
    ${SOURCE_DIR}/src/plugin.cpp
    ${SOURCE_DIR}/src/MockbaModular.cpp
    ${SOURCE_DIR}/src/Blank.cpp
    
    # Faders
    ${SOURCE_DIR}/src/Feidah.cpp
    ${SOURCE_DIR}/src/FeidahS.cpp
    
    # Filters and Effects
    ${SOURCE_DIR}/src/Filtah.cpp
    ${SOURCE_DIR}/src/Mixah.cpp
    ${SOURCE_DIR}/src/Mixah3.cpp
    ${SOURCE_DIR}/src/Pannah.cpp
    
    # Clock and Logic
    ${SOURCE_DIR}/src/Dividah.cpp
    ${SOURCE_DIR}/src/Countah.cpp
    ${SOURCE_DIR}/src/Selectah.cpp
    ${SOURCE_DIR}/src/Shapah.cpp
    ${SOURCE_DIR}/src/Holdah.cpp

    # CZ Series
    ${SOURCE_DIR}/src/CZSaw.cpp
    ${SOURCE_DIR}/src/CZSquare.cpp
    ${SOURCE_DIR}/src/CZPulse.cpp
    ${SOURCE_DIR}/src/CZDblSine.cpp
    ${SOURCE_DIR}/src/CZReso1.cpp
    ${SOURCE_DIR}/src/CZReso2.cpp
    ${SOURCE_DIR}/src/CZReso3.cpp
    ${SOURCE_DIR}/src/CZOsc.cpp
    ${SOURCE_DIR}/src/CZSawPulse.cpp
    
    # Maug Series
    ${SOURCE_DIR}/src/MaugOsc.cpp
    ${SOURCE_DIR}/src/MaugTriangle.cpp
    ${SOURCE_DIR}/src/MaugShark.cpp
    ${SOURCE_DIR}/src/MaugSaw.cpp
    ${SOURCE_DIR}/src/MaugSaw2.cpp
    ${SOURCE_DIR}/src/MaugSquare.cpp
    ${SOURCE_DIR}/src/MaugSquare2.cpp
    ${SOURCE_DIR}/src/MaugSquare3.cpp
    
    # Logic Series
    ${SOURCE_DIR}/src/DualNOT.cpp
    ${SOURCE_DIR}/src/DualBUFFER.cpp
    ${SOURCE_DIR}/src/DualAND.cpp
    ${SOURCE_DIR}/src/DualNAND.cpp
    ${SOURCE_DIR}/src/DualNOR.cpp
    ${SOURCE_DIR}/src/DualOR.cpp
    ${SOURCE_DIR}/src/DualXOR.cpp
    ${SOURCE_DIR}/src/DualXNOR.cpp
    ${SOURCE_DIR}/src/Comparator.cpp
    ${SOURCE_DIR}/src/PSelectah.cpp

    ${SOURCE_DIR}/src/ReVoltah.cpp
)

# Add include directories
target_include_directories(MockbaModular PRIVATE 
    ${SOURCE_DIR}
    ${SOURCE_DIR}/src
    ${DSP_DIR}
)

# Create the plugin file
create_plugin(
    SOURCE_LIB      MockbaModular
    PLUGIN_NAME     MockbaModular
    PLUGIN_JSON     ${SOURCE_DIR}/plugin.json
    SOURCE_ASSETS   ${CMAKE_CURRENT_LIST_DIR}/assets
    DESTINATION     ${CMAKE_CURRENT_LIST_DIR}/metamodule-plugins
) 