add_library(
	veclib
	veclib.c
	veclib.h
)

target_include_directories(veclib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")