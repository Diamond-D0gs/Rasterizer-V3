cmake_minimum_required(VERSION 3.12)
project(G-Rasterizer VERSION 1.0.0)

add_library (
    typedefs
    INTERFACE
    typedefs.h
)

target_include_directories(libbmp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")