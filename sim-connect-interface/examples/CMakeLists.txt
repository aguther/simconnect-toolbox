
include_directories(
        "$ENV{MSFS_SDK}/SimConnect SDK/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

# ---------------------- SimConnectTestRead -----------------------------------

add_executable(
        SimConnectTestRead
        main-read.cpp
)
set_target_properties(
        SimConnectTestRead PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)
target_link_libraries(
        SimConnectTestRead PRIVATE
        SimConnectInterface
        SimConnect
)
add_custom_command(
        TARGET SimConnectTestRead
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectTestRead>/SimConnectTestRead.exe" "${CMAKE_SOURCE_DIR}/matlab"
)

# ---------------------- SimConnectTestWrite ----------------------------------

add_executable(
        SimConnectTestWrite
        main-write.cpp
)
set_target_properties(
        SimConnectTestWrite PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)
target_link_libraries(
        SimConnectTestWrite PRIVATE
        SimConnectInterface
        SimConnect
)
add_custom_command(
        TARGET SimConnectTestWrite
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectTestWrite>/SimConnectTestWrite.exe" "${CMAKE_SOURCE_DIR}/matlab"
)

# ---------------------- SimConnectTestInput ----------------------------------

add_executable(
        SimConnectTestInput
        main-input.cpp
)

set_target_properties(
        SimConnectTestInput PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)

target_link_libraries(
        SimConnectTestInput PRIVATE
        SimConnectInterface
        SimConnect
)

add_custom_command(
        TARGET SimConnectTestInput
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectTestInput>/SimConnectTestInput.exe" "${CMAKE_SOURCE_DIR}/matlab"
)

# ---------------------- SimConnectTestEvent ----------------------------------

add_executable(
        SimConnectTestEvent
        main-event.cpp
)

set_target_properties(
        SimConnectTestEvent PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)

target_link_libraries(
        SimConnectTestEvent PRIVATE
        SimConnectInterface
        SimConnect
)

add_custom_command(
        TARGET SimConnectTestEvent
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectTestInput>/SimConnectTestInput.exe" "${CMAKE_SOURCE_DIR}/matlab"
)
