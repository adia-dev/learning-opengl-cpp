cmake_minimum_required(VERSION 3.26)

project(Glad)

add_library(glad glad.c)
target_include_directories(glad PUBLIC ../../include/)
