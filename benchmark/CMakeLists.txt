find_package(benchmark 1.9 REQUIRED)

add_library(utility STATIC utility.cpp utility.h)

add_executable(rotate_benchmark rotate.cpp)
target_link_libraries(rotate_benchmark PRIVATE rotate utility benchmark::benchmark)

add_executable(reverse_benchmark reverse.cpp)
target_link_libraries(reverse_benchmark PRIVATE rotate utility benchmark::benchmark)
