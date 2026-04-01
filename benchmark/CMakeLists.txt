find_package(benchmark 1.9 REQUIRED)
add_executable(rotate_benchmark rotate.cpp)
target_link_libraries(rotate_benchmark PRIVATE rotate benchmark::benchmark)
