cmake_minimum_required(VERSION 3.26)

project(StbImage)

add_library(stb_image stb_image.cpp)
target_include_directories(glad PUBLIC ../../include/)
