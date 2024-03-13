cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME FirstOpenGLProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RESOURCES_PATH "${CMAKE_CURRENT_BINARY_DIR}/res/")


project(${PROJECT_NAME} DESCRIPTION "OPENGL Experiments" LANGUAGES CXX)

find_package(glfw3 3.3 REQUIRED)

add_compile_definitions(GLFW_INCLUDE_NONE)
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")

file(GLOB_RECURSE SOURCES src/*.cpp src/**/*.cpp)
file(GLOB_RECURSE RESOURCES res/*)

foreach(RESOURCE ${RESOURCES})
    file(RELATIVE_PATH RELATIVE_PATH_RES ${CMAKE_SOURCE_DIR} ${RESOURCE})
    configure_file(${RESOURCE} ${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PATH_RES} COPYONLY)
endforeach()

add_executable(app ${SOURCES})
add_subdirectory(vendor/)

target_include_directories(app PUBLIC src glfw deps include)
target_link_libraries(app PUBLIC glfw glad stb_image)
