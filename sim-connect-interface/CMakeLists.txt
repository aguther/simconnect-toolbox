include_directories(
        "$ENV{MSFS_SDK}/SimConnect SDK/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

link_directories(
        "$ENV{MSFS_SDK}/SimConnect SDK/lib"
)

add_definitions(
        -DWIN32_LEAN_AND_MEAN
        -DNOMINMAX
        -DNOGDI
)

add_library(
        SimConnectInterface SHARED
        include/MemoryAccessor.h
        include/SimConnectData.h
        include/SimConnectDataDefinition.h
        include/SimConnectDataInterface.h
        include/SimConnectEventInterface.h
        include/SimConnectInputInterface.h
        include/SimConnectVariable.h
        include/SimConnectVariableLookupTable.h
        include/SimConnectVariableParser.h
        include/SimConnectVariableType.h
        src/SimConnectData.cpp
        src/SimConnectDataDefinition.cpp
        src/SimConnectDataInterface.cpp
        src/SimConnectEventInterface.cpp
        src/SimConnectInputInterface.cpp
        src/SimConnectVariableLookupTable.cpp
)

target_link_libraries(
        SimConnectInterface PRIVATE
        SimConnect
)

add_custom_command(
        TARGET SimConnectInterface
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectInterface>/SimConnectInterface.dll" "${CMAKE_SOURCE_DIR}/matlab"
)
