#include "rotate/functors.h"
#include "rotate/rotate.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <numeric>

namespace {
template <class Impl>
void expect_rotate()
{
    constexpr std::size_t size = 21;
    for (std::size_t i = 0; i < size; ++i)
    {
        std::array<int, size> got;
        std::ranges::iota(got, 0);
        std::array expected = got;
        auto got_it = Impl{}(std::begin(got), std::begin(got) + i, std::end(got));
        auto expected_it = std::rotate(std::begin(expected), std::begin(expected) + i, std::end(expected));
        EXPECT_EQ(got_it - std::begin(got), expected_it - std::begin(expected));
        EXPECT_EQ(got, expected);
    }
}
} // namespace

#define ROTATE_TEST(impl)                                                                                              \
    TEST(RotateTest, impl)                                                                                             \
    {                                                                                                                  \
        expect_rotate<rks::impl>();                                                                                    \
    }

ROTATE_TEST(ThreeReverse)
ROTATE_TEST(Forward)
ROTATE_TEST(ForwardOptimised)
