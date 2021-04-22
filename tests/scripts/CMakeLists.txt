cmake_minimum_required(VERSION 3.12)

project(Scripts VERSION 1.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB_RECURSE Scripts_SRC
"*.cpp"
"*.hpp"
"*.h")


add_library(${PROJECT_NAME} SHARED
 ${Scripts_SRC})

 target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "../../include"
    "../include"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/bin"
        "../bin")

