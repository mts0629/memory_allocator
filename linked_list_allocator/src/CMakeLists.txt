cmake_minimum_required(VERSION 3.22)

file(GLOB SOURCES *.c)

set(CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED True)

add_library(${TARGET_LIB_NAME} SHARED ${SOURCES})

target_compile_options(${TARGET_LIB_NAME}
	PUBLIC -Wall -Wextra -Wpedantic -Werror
)

if(DEFINED HEAP_SIZE) 
	target_compile_definitions(${TARGET_LIB_NAME} PRIVATE HEAP_SIZE=${HEAP_SIZE})
endif()

target_include_directories(${TARGET_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
