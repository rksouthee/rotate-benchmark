cmake_minimum_required(VERSION 4.2)

project(RotateBenchmark LANGUAGES CXX)

add_library(rotate INTERFACE)
target_sources(rotate
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS include
	FILES
	include/rotate/rotate.h
)

add_subdirectory(benchmark)
add_subdirectory(tests)
