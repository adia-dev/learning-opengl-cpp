cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME FirstOpenGLProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(${PROJECT_NAME} DESCRIPTION "OPENGL Experiments" LANGUAGES CXX)

# GLFW
find_package(glfw3 3.3 REQUIRED)

add_compile_definitions(GLFW_INCLUDE_NONE)

file(GLOB_RECURSE SOURCES src/*.cpp src/**/*.cpp)
file(GLOB SHADERS "shaders/**/*")
file(GLOB RESOURCES "res/**/*")

foreach(SHADER ${SHADERS})
    configure_file(${SHADER} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

foreach(RES ${RESOURCES})
    configure_file(${RES} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

add_executable(app ${SOURCES})
add_subdirectory(vendor/)

target_include_directories(app PUBLIC src glfw deps include)
target_link_libraries(app PUBLIC glfw glad stb_image)
