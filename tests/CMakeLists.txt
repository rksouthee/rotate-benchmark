enable_testing()

find_package(GTest 1.17 REQUIRED)

include(GoogleTest)

add_executable(rotate_test rotate_test.cpp)
target_link_libraries(rotate_test PRIVATE rotate GTest::gtest_main)
gtest_discover_tests(rotate_test)
