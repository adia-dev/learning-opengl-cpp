cmake_minimum_required(VERSION 3.26)

project(imgui)

file(GLOB_RECURSE SOURCES *.cpp)
find_package(glfw3 3.3 REQUIRED)


add_library(imgui ${SOURCES})
target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC . ../../include/ glfw)
