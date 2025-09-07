cmake_minimum_required(VERSION 3.22)

project(bump_allocator)

set(TARGET_LIB_NAME bump_allocator)

add_subdirectory(src)

add_executable(test test.c)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test PRIVATE ${TARGET_LIB_NAME})
