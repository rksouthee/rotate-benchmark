#include "utility.h"

#include "rotate/functors.h"

#include <benchmark/benchmark.h>

#include <cstdint>

namespace {
template <class Impl, class T>
void BM_reverse(benchmark::State& state)
{
    const auto n = static_cast<int>(state.range(0));
    std::vector<T> v(n);
    fill(v);

    T* first = v.data();
    T* last = first + n;

    const Impl impl{};
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(v.data());

        impl(first, last);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::uint64_t>(n));
}

void reverse_args(benchmark::internal::Benchmark* b)
{
    for (int n : {8, 16, 32, 64, 128, 256, 1024, 4096, 16384, 2097152})
    {
        b->Args({n});
    }
}
} // namespace

#define REVERSE_BENCHMARK(impl) BENCHMARK_TEMPLATE(BM_reverse, impl, std::uint32_t)->Apply(reverse_args)

REVERSE_BENCHMARK(rks::StdReverse);
REVERSE_BENCHMARK(rks::RksReverse);

BENCHMARK_MAIN();
