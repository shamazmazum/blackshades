cmake_minimum_required (VERSION 2.6)
cmake_policy(SET CMP0048 NEW)

project (blackshades VERSION 0.2)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules")
find_package (OpenGL REQUIRED)
find_package (OpenAL REQUIRED)
find_package (Alut REQUIRED)
find_package (Vorbis REQUIRED)
find_package (SDL2 REQUIRED)

add_subdirectory (Source)
