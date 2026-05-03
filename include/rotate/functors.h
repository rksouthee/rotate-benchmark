#pragma once

#include "rotate/algorithm.h"
#include "rotate/rotate.h"

#include <algorithm>

namespace rks {
struct StdRotate
{
    template <typename I>
    I operator()(I first, I middle, I last) const
    {
        return std::rotate(first, middle, last);
    }
};

struct ThreeReverse
{
    template <typename I>
    I operator()(I first, I middle, I last) const
    {
        return rks::three_reverse(first, middle, last);
    }
};

struct Forward
{
    template <typename I>
    [[nodiscard]] I operator()(I first, I middle, I last) const
    {
        return rks::forward(first, middle, last);
    }
};

struct ForwardOptimised
{
    template <typename I>
    I operator()(I first, I middle, I last) const
    {
        return rks::forward_optimised(first, middle, last);
    }
};

struct StdReverse
{
    template <typename I>
    void operator()(I first, I last) const
    {
        std::reverse(first, last);
    }
};

struct RksReverse
{
    template <typename I>
    void operator()(I first, I last) const
    {
        rks::reverse(first, last);
    }
};
} // namespace rks
