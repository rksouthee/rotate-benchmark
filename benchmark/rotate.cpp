#include "rotate/rotate.h"

#include <benchmark/benchmark.h>

#include <array>

namespace {
struct Trivial64
{
    std::uint64_t values[8];
};

void fill(std::vector<std::uint32_t>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        v[i] = static_cast<std::uint32_t>(i);
    }
}

void fill(std::vector<std::string>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        // TODO: Should we avoid Small String Optimisation
        v[i] = "string " + std::to_string(i);
    }
}

void fill(std::vector<Trivial64>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        Trivial64& value = v[i];
        constexpr std::size_t size = std::size(value.values);
        for (std::size_t j = 0; j < size; ++j)
        {
            value.values[j] = (i * size) + j;
        }
    }
}

enum class Mode
{
    k_one = 0,
    quarter = 1,
    half = 2,
    three_quarter = 3,
    n_minus_one = 4,
};

template <class T>
void BM_std_rotate(benchmark::State& state)
{
    const auto n = static_cast<int>(state.range(0));
    const auto mode = static_cast<Mode>(state.range(1));

    std::vector<T> v(n);
    fill(v);

    int k = 0;
    switch (mode)
    {
        using enum Mode;
    case k_one:
        k = 1;
        break;
    case quarter:
        k = n / 4;
        break;
    case half:
        k = n / 2;
        break;
    case three_quarter:
        k = (3 * n) / 4;
        break;
    case n_minus_one:
        k = n > 0 ? n - 1 : 0;
        break;
    default:
        k = n / 2;
        break;
    }

    if (n == 0)
    {
        k = 0;
    }

    const auto first = v.begin();
    const auto last = v.end();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(v.data());

        std::rotate(first, first + k, last);
        std::rotate(first, first + (n - k), last);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::uint64_t>(n));
}

void rotate_args(benchmark::internal::Benchmark* b)
{
    // TODO: Bigger sizes
    for (int n : {8, 16, 32, 64, 128, 256, 1024, 4096, 16384})
    {
        for (int mode = 0; mode <= 4; ++mode)
        {
            b->Args({n, mode});
        }
    }
}

#define ROTATE_BENCHMARK(func)                                                                                         \
    BENCHMARK_TEMPLATE(func, std::uint32_t)->Apply(rotate_args);                                                       \
    BENCHMARK_TEMPLATE(func, Trivial64)->Apply(rotate_args);                                                           \
    BENCHMARK_TEMPLATE(func, std::string)->Apply(rotate_args)

ROTATE_BENCHMARK(BM_std_rotate);
} // namespace

BENCHMARK_MAIN();
