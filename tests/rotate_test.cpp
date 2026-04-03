#include "rotate/rotate.h"

#include <gtest/gtest.h>

#include <array>

TEST(RotateTest, Rotate)
{
    std::array got = {1, 3, 1, 4, 1, 5};
    std::array expected = got;
    constexpr std::ptrdiff_t k = 3;
    auto got_it = rks::rotate(std::begin(got), std::begin(got) + k, std::end(got));
    auto expected_it = std::rotate(std::begin(expected), std::begin(expected) + k, std::end(expected));
    EXPECT_EQ(got_it - std::begin(got), expected_it - std::begin(expected));
    EXPECT_EQ(got, expected);
}
