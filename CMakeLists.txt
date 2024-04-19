cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)

project("bmx")

add_library(
    "bmx" STATIC
    src/bmx.cpp
)

target_include_directories(
    "bmx" PUBLIC
    include/
)