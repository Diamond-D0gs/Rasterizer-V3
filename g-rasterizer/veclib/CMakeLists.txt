cmake_minimum_required(VERSION 3.12)
project(G-Rasterizer VERSION 1.0.0)

add_library(
	veclib
	veclib.c
	veclib.h
)

target_include_directories(veclib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")