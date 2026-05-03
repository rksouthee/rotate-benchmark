#pragma once

#include <algorithm>

#include <cstddef>
#include <cstdint>

#include <immintrin.h>

namespace rks {
template <typename I, typename N>
void reverse_n(I f, N n)
{
    if (n == 0)
    {
        return;
    }
    N i{0};
    --n;
    while (i < n)
    {
        std::iter_swap(f + i, f + n);
        ++i;
        --n;
    }
}

inline __m256i reverse_8x32(__m256i v)
{
    const __m256i idx = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    return _mm256_permutevar8x32_epi32(v, idx);
}

inline void reverse_uint32_avx2(std::uint32_t* f, std::ptrdiff_t n)
{
    std::ptrdiff_t i = 0;
    std::ptrdiff_t j = n;

    while (j - i >= 16)
    {
        j -= 8;
        __m256i left = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(f + i));
        __m256i right = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(f + j));

        left = reverse_8x32(left);
        right = reverse_8x32(right);

        _mm256_storeu_si256(reinterpret_cast<__m256i*>(f + i), right);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(f + j), left);

        i += 8;
    }

    reverse_n(f + i, j - i);
}

template <typename T>
void reverse_n_simd(T* p, std::ptrdiff_t n)
{
    if constexpr (std::is_same_v<T, std::uint32_t>)
    {
        reverse_uint32_avx2(p, n);
    }
    else
    {
        reverse_n(p, n);
    }
}

template <typename I>
void reverse(I first, I last)
{
    reverse_n_simd(first, last - first);
}

} // namespace rks
