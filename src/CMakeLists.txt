cmake_minimum_required(VERSION 3.6)

project(AWML)

include_directories("../include/AWML")

file(GLOB INCLUDES "*.h")
list(APPEND FILES ${INCLUDES})

if (WIN32)
    list(APPEND FILES awml_windows.cpp)
    list(REMOVE_ITEM FILES awml_xwindow.h)
    list(APPEND FILES awml.cpp)
    add_library(AWML STATIC ${FILES})
elseif (UNIX)
    list(APPEND FILES awml_xwindow.cpp)
    list(REMOVE_ITEM FILES awml_windows.h)
    list(APPEND FILES awml.cpp)
    add_library(AWML STATIC ${FILES})
    target_link_libraries(AWML -lX11)
endif()

