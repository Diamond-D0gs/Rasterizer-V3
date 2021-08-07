add_executable(g-raster main.c)

target_link_libraries(g-raster PRIVATE veclib)
target_link_libraries(g-raster PRIVATE libbmp)