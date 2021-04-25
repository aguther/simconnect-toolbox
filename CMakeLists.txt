cmake_minimum_required(VERSION 3.17)
project(SimConnectToolbox LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../blockfactory/install")

find_package(
        BlockFactory REQUIRED COMPONENTS Core Simulink
)

include_directories(
        "$ENV{MSFS_SDK}/SimConnect SDK/include"
        ${CMAKE_SOURCE_DIR}/sim-connect-interface/include
        ${CMAKE_SOURCE_DIR}/src/SimConnectInput
        ${CMAKE_SOURCE_DIR}/src/SimConnectSink
        ${CMAKE_SOURCE_DIR}/src/SimConnectSinkAutopilotLaws
        ${CMAKE_SOURCE_DIR}/src/SimConnectSinkAutopilotStateMachine
        ${CMAKE_SOURCE_DIR}/src/SimConnectSinkAutothrust
        ${CMAKE_SOURCE_DIR}/src/SimConnectSinkFlyByWire
        ${CMAKE_SOURCE_DIR}/src/SimConnectSource
        ${CMAKE_SOURCE_DIR}/src/SimConnectSourceAutopilotEvents
        ${CMAKE_SOURCE_DIR}/src/SimConnectSourceAutopilotLaws
        ${CMAKE_SOURCE_DIR}/src/SimConnectSourceAutopilotStateMachine
        ${CMAKE_SOURCE_DIR}/src/SimConnectSourceFlyByWire
        ${CMAKE_SOURCE_DIR}/src/SimConnectSourceLocalVariables
        ${CMAKE_SOURCE_DIR}/src/SimConnectSourceLocalVariablesAutothrust
)

link_directories(
        "$ENV{MSFS_SDK}/SimConnect SDK/lib"
)

add_definitions(
        -DWIN32_LEAN_AND_MEAN
        -DNOMINMAX
        -DNOGDI
)

add_subdirectory(sim-connect-interface)
add_subdirectory(sim-connect-interface/examples)

add_library(
        SimConnectToolbox SHARED
        src/Factory/Factory.cpp
        src/SimConnectInput/SimConnectInput.cpp
        src/SimConnectSink/SimConnectSink.cpp
        src/SimConnectSinkAutopilotLaws/SimConnectSinkAutopilotLaws.cpp
        src/SimConnectSinkAutopilotStateMachine/SimConnectSinkAutopilotStateMachine.cpp
        src/SimConnectSinkAutothrust/SimConnectSinkAutothrust.cpp
        src/SimConnectSinkFlyByWire/SimConnectSinkFlyByWire.cpp
        src/SimConnectSource/SimConnectSource.cpp
        src/SimConnectSourceAutopilotEvents/SimConnectSourceAutopilotEvents.cpp
        src/SimConnectSourceAutopilotLaws/SimConnectSourceAutopilotLaws.cpp
        src/SimConnectSourceAutopilotStateMachine/SimConnectSourceAutopilotStateMachine.cpp
        src/SimConnectSourceFlyByWire/SimConnectSourceFlyByWire.cpp
        src/SimConnectSourceLocalVariables/SimConnectSourceLocalVariables.cpp
        src/SimConnectSourceLocalVariablesAutothrust/SimConnectSourceLocalVariablesAutothrust.cpp
)

set_target_properties(
        SimConnectToolbox PROPERTIES
        OUTPUT_NAME "SimConnectToolbox"
)

target_link_libraries(
        SimConnectToolbox PRIVATE
        BlockFactory::Core
        SimConnectInterface
        SimConnect
)

add_custom_command(
        TARGET SimConnectToolbox
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{MSFS_SDK}/SimConnect SDK/lib/SimConnect.dll" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/external/SimConnect/SimConnect.cfg" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/../blockfactory/install/bin/BlockFactoryCore.dll" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/../blockfactory/install/bin/mxpp.dll" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/../blockfactory/install/bin/shlibpp.dll" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/../blockfactory/install/mex/BlockFactory.mexw64" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectInterface>/SimConnectInterface.dll" "${CMAKE_SOURCE_DIR}/matlab"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectToolbox>/SimConnectToolbox.dll" "${CMAKE_SOURCE_DIR}/matlab"
)
