project(LIB)
SET(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 2.8)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB LIB ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(LIB ${LIB})

add_library(libconfig_x86.a ${LIB})
set_target_properties(libconfig_x86.a PROPERTIES OUTPUT_NAME config_x86)
