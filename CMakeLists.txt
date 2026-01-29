cmake_minimum_required(VERSION 3.10)
project(custom_malloc C)

set(CMAKE_C_STANDARD 11)


add_library(embedded_malloc STATIC src/embedded_malloc.c)


add_executable(test_allocator tests/test.c)
target_link_libraries(test_allocator embedded_malloc)