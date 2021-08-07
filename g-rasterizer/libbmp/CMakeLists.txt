add_library(
	libbmp
	libbmp.c
	libbmp.h
)

target_include_directories(libbmp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")