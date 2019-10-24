cmake_minimum_required(VERSION 3.6)

project(AWML)

include_directories("../include/AWML")

if (WIN32)
    set(FILES awml_windows.cpp)
endif()

list(APPEND FILES awml.cpp)

file(GLOB INCLUDES "*.h")
list(APPEND FILES ${INCLUDES})

add_library(AWML STATIC ${FILES})