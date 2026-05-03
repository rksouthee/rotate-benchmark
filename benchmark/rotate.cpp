#include "utility.h"

#include "rotate/functors.h"
#include "rotate/rotate.h"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace {
enum class Mode
{
    k_one = 0,
    quarter = 1,
    half = 2,
    three_quarter = 3,
    n_minus_one = 4,
};

template <class Impl, class T>
void BM_rotate(benchmark::State& state)
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

    T* first = v.data();
    T* last = first + n;

    const Impl impl{};
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(v.data());

        (void)impl(first, first + k, last);
        (void)impl(first, first + (n - k), last);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::uint64_t>(n));
}

void rotate_args(benchmark::internal::Benchmark* b)
{
    // TODO: Bigger sizes
    for (int n : {8, 16, 32, 64, 128, 256, 1024, 4096, 16384, 2097152})
    {
        for (int mode = 0; mode <= 4; ++mode)
        {
            b->Args({n, mode});
        }
    }
}

#define ROTATE_BENCHMARK(impl)                                                                                         \
    BENCHMARK_TEMPLATE(BM_rotate, impl, std::uint32_t)->Apply(rotate_args);                                            \
    BENCHMARK_TEMPLATE(BM_rotate, impl, Trivial64)->Apply(rotate_args);                                                \
    BENCHMARK_TEMPLATE(BM_rotate, impl, std::string)->Apply(rotate_args)

ROTATE_BENCHMARK(rks::StdRotate);
ROTATE_BENCHMARK(rks::ThreeReverse);
ROTATE_BENCHMARK(rks::Forward);
ROTATE_BENCHMARK(rks::ForwardOptimised);
} // namespace

BENCHMARK_MAIN();
