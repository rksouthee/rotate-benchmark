#include "rotate/algorithm.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <numeric>

TEST(ReverseTest, reverse_uint32_with_simd)
{
    constexpr std::size_t size = 21;
    std::array<std::uint32_t, size> got;
    std::ranges::iota(got, 0);
    std::array expected = got;
    rks::reverse_uint32_avx2(got.data(), std::size(got));
    std::reverse(std::begin(expected), std::end(expected));
    EXPECT_EQ(got, expected);
}
