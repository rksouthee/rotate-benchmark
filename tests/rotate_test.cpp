#include "rotate/rotate.h"

#include <gtest/gtest.h>

#include <array>

TEST(RotateTest, Rotate)
{
    std::array data = {1, 3, 1, 4, 1, 5};
    auto result = rks::rotate(std::begin(data), std::begin(data) + 3, std::end(data));
    EXPECT_EQ(result, std::begin(data) + 3);
}
