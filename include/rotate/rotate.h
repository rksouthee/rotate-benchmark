#pragma once

#include "rotate/algorithm.h"

#include <algorithm>
#include <concepts>
#include <iterator>

#include <cassert>

namespace rks {
template <typename I>
    requires std::random_access_iterator<I> && std::indirectly_movable_storable<I, I> && std::indirectly_swappable<I>
[[nodiscard]] I three_reverse(I first, I middle, I last)
{
    if constexpr (std::is_same_v<I, std::uint32_t*>)
    {
        rks::reverse_uint32_avx2(first, middle - first);
        rks::reverse_uint32_avx2(middle, last - middle);
        rks::reverse_uint32_avx2(first, last - first);
    }
    else
    {
        rks::reverse_n(first, middle - first);
        rks::reverse_n(middle, last - middle);
        rks::reverse_n(first, last - first);
    }
    return first + (last - middle);
}

template <typename I0, typename I1>
    requires std::bidirectional_iterator<I0> && std::bidirectional_iterator<I1> && std::indirectly_swappable<I0, I1>
[[nodiscard]] I1 swap_ranges_backward(I0 f0, I0 l0, I1 l1)
{
    while (l0 != f0)
    {
        --l0;
        --l1;
        std::ranges::iter_swap(l0, l1);
    }

    return l1;
}

template <std::permutable I>
void rotate_forward_step(I& f, I& m, I& l)
{
    I c = m;
    do
    {
        std::ranges::iter_swap(f, c);
        ++f;
        ++c;
        if (f == m)
        {
            m = c;
        }
    } while (c != l);
}

template <std::permutable I>
[[nodiscard]] I forward(I f, I m, I l)
{
    if (m == f)
    {
        return l;
    }

    if (m == l)
    {
        return f;
    }

    rks::rotate_forward_step(f, m, l);
    const I m_prime = f;
    while (m != l)
    {
        rks::rotate_forward_step(f, m, l);
    }

    return m_prime;
}

template <typename I0, typename I1>
std::pair<I0, I1> swap_ranges_bounded(I0 f0, I0 l0, I1 f1, I1 l1)
{
    while (f0 != l0 && f1 != l1)
    {
        std::ranges::iter_swap(f0, f1);
        ++f0;
        ++f1;
    }

    return {f0, f1};
}

template <typename I0, typename I1, typename N>
std::pair<I0, I1> swap_ranges_n(I0 f0, N n, I1 f1)
{
    while (n >= N(4))
    {
        std::ranges::iter_swap(f0, f1);
        ++f0;
        ++f1;
        std::ranges::iter_swap(f0, f1);
        ++f0;
        ++f1;
        std::ranges::iter_swap(f0, f1);
        ++f0;
        ++f1;
        std::ranges::iter_swap(f0, f1);
        ++f0;
        ++f1;
        n -= N(4);
    }

    while (n != N(0))
    {
        std::ranges::iter_swap(f0, f1);
        ++f0;
        ++f1;
        --n;
    }

    return {f0, f1};
}

template <typename I0, typename I1, typename N>
std::pair<I0, I1> swap_ranges_backward_n(I0 l0, N n, I1 l1)
{
    while (n >= N(4))
    {
        --l0;
        --l1;
        std::ranges::iter_swap(l0, l1);
        --l0;
        --l1;
        std::ranges::iter_swap(l0, l1);
        --l0;
        --l1;
        std::ranges::iter_swap(l0, l1);
        --l0;
        --l1;
        std::ranges::iter_swap(l0, l1);
        n -= N(4);
    }

    while (n != N(0))
    {
        --l0;
        --l1;
        std::ranges::iter_swap(l0, l1);
        --n;
    }

    return {l0, l1};
}

template <typename I>
    requires std::random_access_iterator<I> && std::indirectly_movable_storable<I, I> && std::indirectly_swappable<I>
[[nodiscard]] I forward_optimised(I f, I m, I l)
{
    if (m == f)
    {
        return l;
    }

    if (m == l)
    {
        return f;
    }

    using N = std::iterator_traits<I>::difference_type;

    N a = m - f;
    N b = l - m;
    const I m_prime = f + b;

    while (true)
    {
        if (a <= b)
        {
            const N n = b / a;
            const std::pair<I, I> p = rks::swap_ranges_n(f, n * a, m);
            f = p.first;
            m = p.second;
            b = b % a;
            if (b == 0)
            {
                return m_prime;
            }
        }
        else
        {
            const N n = a / b;
            // This might not be a good idea as we aren't as cache friendly compared to iterating iteratively
            const std::pair<I, I> p = rks::swap_ranges_backward_n(m, n * b, l);
            m = p.first;
            l = p.second;
            a = a % b;
            if (a == 0)
            {
                return m_prime;
            }
        }
    }
}
} // namespace rks
