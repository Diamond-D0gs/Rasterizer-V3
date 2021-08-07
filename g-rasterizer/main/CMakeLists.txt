cmake_minimum_required(VERSION 3.12)
project(G-Rasterizer VERSION 1.0.0)

add_executable(g-raster main.c)

target_link_libraries(g-raster PRIVATE veclib)
target_link_libraries(g-raster PRIVATE libbmp)