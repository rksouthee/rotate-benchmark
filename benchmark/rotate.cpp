#include <benchmark/benchmark.h>

#include "rotate/rotate.h"

#include <array>

namespace {
static void BM_rotate(benchmark::State& state)
{
    std::array data = {1, 3, 1, 4, 1, 5};
    for (auto _ : state)
    {
        (void)rks::rotate(std::begin(data), std::begin(data) + 3, std::end(data));
    }
}
} // namespace

BENCHMARK(BM_rotate);

BENCHMARK_MAIN();
