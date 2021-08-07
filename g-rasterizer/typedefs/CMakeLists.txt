add_library (
    typedefs
    INTERFACE
    typedefs.h
)

target_include_directories(libbmp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")