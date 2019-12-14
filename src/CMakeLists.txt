cmake_minimum_required(VERSION 3.6)

project(AWML)

set(PROJECT_ROOT ../)

if (WIN32)
    include_directories("${$PROJECT_ROOT}/include" "${PROJECT_ROOT}/include/AWML")
    execute_process(
        COMMAND python ${PROJECT_ROOT}/dependency_downloader.py ${PROJECT_ROOT}/include
        RESULT_VARIABLE FAILED
    )
    if (FAILED)
        message(FATAL_ERROR "-- Build cancelled since dependencies couldn't be donwloaded.")
    endif()
    file(GLOB AWML_SRC "Windows*")
    list(APPEND AWML_SRC "awml.cpp")
    add_library(AWML STATIC ${AWML_SRC})
    target_link_libraries(AWML Opengl32)
elseif (UNIX)
    file(GLOB AWML_SRC "X*")
    list(APPEND AWML_SRC "awml.cpp")
    add_library(AWML STATIC ${AWML_SRC})
    target_link_libraries(AWML X11 GL)
endif()

