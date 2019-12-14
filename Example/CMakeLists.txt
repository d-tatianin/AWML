cmake_minimum_required(VERSION 3.6)

project(AWMLExample)
include_directories("../include")
set(CMAKE_BUILD_TYPE Release)

add_executable(AWMLExample MyApplication.cpp)
add_subdirectory(../src AWML)

target_link_libraries(AWMLExample AWML)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AWMLExample)
