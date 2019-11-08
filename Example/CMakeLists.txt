cmake_minimum_required(VERSION 3.6)

project(AWMLExample)
include_directories("../include")
set(CMAKE_BUILD_TYPE Release)

add_executable(AWMLExample MyApplication.cpp)
add_subdirectory(../src AWML)

if(UNIX)
  target_link_libraries(AWMLExample GL)
elseif(WIN32)
  target_link_libraries(AWMLExample Opengl32)
endif()

target_link_libraries(AWMLExample AWML)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AWMLExample)
