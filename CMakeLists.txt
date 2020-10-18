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
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/SimConnectInput
        ${CMAKE_SOURCE_DIR}/src/SimConnectInterface
        ${CMAKE_SOURCE_DIR}/src/SimConnectSink
        ${CMAKE_SOURCE_DIR}/src/SimConnectSource
)
link_directories(
        "$ENV{MSFS_SDK}/SimConnect SDK/lib"
        ${CMAKE_SOURCE_DIR}/external/SimConnect
)

add_library(
        SimConnectToolbox SHARED
        src/Factory/Factory.cpp
        src/SimConnectInput/SimConnectInput.h
        src/SimConnectInput/SimConnectInput.cpp
        src/SimConnectInterface/MemoryAccessor.h
        src/SimConnectInterface/SimConnectVariableParser.h
        src/SimConnectInterface/SimConnectData.h
        src/SimConnectInterface/SimConnectData.cpp
        src/SimConnectInterface/SimConnectDataDefinition.h
        src/SimConnectInterface/SimConnectDataDefinition.cpp
        src/SimConnectInterface/SimConnectDataInterface.h
        src/SimConnectInterface/SimConnectDataInterface.cpp
        src/SimConnectInterface/SimConnectInputInterface.h
        src/SimConnectInterface/SimConnectInputInterface.cpp
        src/SimConnectInterface/SimConnectVariable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.cpp
        src/SimConnectInterface/SimConnectVariableType.h
        src/SimConnectSink/SimConnectSink.h
        src/SimConnectSink/SimConnectSink.cpp
        src/SimConnectSource/SimConnectSource.h
        src/SimConnectSource/SimConnectSource.cpp
)
set_target_properties(
        SimConnectToolbox PROPERTIES
        OUTPUT_NAME "SimConnectToolbox"
)
target_link_libraries(
        SimConnectToolbox PRIVATE
        BlockFactory::Core
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
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectToolbox>/SimConnectToolbox.dll" "${CMAKE_SOURCE_DIR}/matlab"
)

add_executable(
        SimConnectTestRead
        src/SimConnectInterface/example/main-read.cpp
        src/SimConnectInterface/MemoryAccessor.h
        src/SimConnectInterface/SimConnectVariableParser.h
        src/SimConnectInterface/SimConnectData.h
        src/SimConnectInterface/SimConnectData.cpp
        src/SimConnectInterface/SimConnectDataDefinition.h
        src/SimConnectInterface/SimConnectDataDefinition.cpp
        src/SimConnectInterface/SimConnectDataInterface.h
        src/SimConnectInterface/SimConnectDataInterface.cpp
        src/SimConnectInterface/SimConnectVariable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.cpp
        src/SimConnectInterface/SimConnectVariableType.h
)
set_target_properties(
        SimConnectTestRead PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)
target_link_libraries(
        SimConnectTestRead PRIVATE
        SimConnect
)

add_executable(
        SimConnectTestWrite
        src/SimConnectInterface/example/main-write.cpp
        src/SimConnectInterface/MemoryAccessor.h
        src/SimConnectInterface/SimConnectVariableParser.h
        src/SimConnectInterface/SimConnectData.h
        src/SimConnectInterface/SimConnectData.cpp
        src/SimConnectInterface/SimConnectDataDefinition.h
        src/SimConnectInterface/SimConnectDataDefinition.cpp
        src/SimConnectInterface/SimConnectDataInterface.h
        src/SimConnectInterface/SimConnectDataInterface.cpp
        src/SimConnectInterface/SimConnectVariable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.cpp
        src/SimConnectInterface/SimConnectVariableType.h
)
set_target_properties(
        SimConnectTestWrite PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)
target_link_libraries(
        SimConnectTestWrite PRIVATE
        SimConnect
)

add_executable(
        SimConnectTestInput
        src/SimConnectInterface/example/main-input.cpp
        src/SimConnectInterface/MemoryAccessor.h
        src/SimConnectInterface/SimConnectVariableParser.h
        src/SimConnectInterface/SimConnectData.h
        src/SimConnectInterface/SimConnectData.cpp
        src/SimConnectInterface/SimConnectDataDefinition.h
        src/SimConnectInterface/SimConnectDataDefinition.cpp
        src/SimConnectInterface/SimConnectInputInterface.h
        src/SimConnectInterface/SimConnectInputInterface.cpp
        src/SimConnectInterface/SimConnectVariable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.h
        src/SimConnectInterface/SimConnectVariableLookupTable.cpp
        src/SimConnectInterface/SimConnectVariableType.h
)
set_target_properties(
        SimConnectTestInput PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)
target_link_libraries(
        SimConnectTestInput PRIVATE
        SimConnect
)
