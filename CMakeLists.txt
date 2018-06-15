cmake_minimum_required(VERSION 3.11)

project(MeteorBladeEnhancer)

SET(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GameExe "" CACHE FILEPATH "游戏Exe路径")
set(GameStartupArgs "w" CACHE STRING "游戏启动参数")

add_definitions("/MP")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (EXTRADXSDK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ExtraDxSDK/Include)
set (EXTRADXSDK_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ExtraDxSDK/Lib)

get_filename_component(GameDir ${GameExe} DIRECTORY)



include_directories(.)

add_subdirectory(ddfix)
add_subdirectory(minhook)

set_property(TARGET ddfix PROPERTY VS_GLOBAL_GameExe "${GameExe}")
set_property(TARGET ddfix PROPERTY VS_GLOBAL_GameDir "${GameDir}")
set_property(TARGET ddfix PROPERTY VS_GLOBAL_GameStartupArgs "${GameStartupArgs}")

