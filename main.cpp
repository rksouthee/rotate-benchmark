#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "rotate.h"

/*
 * TODO
 * print percentage
 * get results for multiple types (use 4, 8, 24) int, double, string/vector
 * use 30%, 60% and 90%
 */

template <typename I, typename O>
O copy(I f_i, I l_i, O f_o)
{
    while (f_i != l_i) *f_o++ = *f_i++;
    return f_o;
}

struct timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;

    void start()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    double stop()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start);
        return delta.count();
    }
};

template <typename T>
double time_rotate(T* f, T* m, T* l, T* (*rotate)(T*, T*, T*), T* buffer, size_t n)
{
    timer t;
    t.start();
    while (n--) {
        copy(buffer, buffer + (l - f), f);
        T* tmp = rotate(f, m, l);
    }
    return t.stop();
}

template <typename T, typename G>
void test_rotate(size_t min_size, size_t max_size, double percent, G gen)
{
    T* (*f_pointers[])(T*, T*, T*) = {
        rotate_forward<T*>,
        rotate_forward_optimized<T*>,
        rotate_bidirectional<T*>,
        rotate_random_access<T*>,
    };

    std::cout << "Element size = " << sizeof(T) << '\n';
    std::cout << "Rotation amount = ";
    std::cout << std::setprecision(3) << (percent * 100.0) << "%\n";
    size_t number_of_rotates = sizeof(f_pointers) / sizeof(f_pointers[0]);
    int width = 14;
    const char title[] = "Rotate Benchmark";
    int center = ((number_of_rotates * width + 12) + sizeof(title)) / 2;
    std::cout << std::setw(center) << title << '\n';
    std::cout <<
        std::setw(12) << "size" <<
        std::setw(width) << "forward" <<
        std::setw(width) << "optimized" <<
        std::setw(width) << "bidirectional" <<
        std::setw(width) << "random" <<
        '\n';

    for (size_t size(min_size); size <= max_size; size *= 2) {
        std::cout << std::setw(12) << size;
        size_t n = max_size / size;
        T* buffer = new T[size];
        gen(buffer, buffer + size);
        T* data = new T[size];
        for (size_t i = 0; i < number_of_rotates; ++i) {
            T* m = data + size_t(size * percent);
            double time = time_rotate(data, m, data + size, f_pointers[i], buffer, n);
            time /= (double(size * n));
            std::cout << std::setw(width) << std::fixed << std::setprecision(0) << time;
        }
        std::cout << '\n';
        delete[] buffer;
        delete[] data;
    }
    const char footer[] = "ns per element";
    center = ((number_of_rotates * width + 12) + sizeof(footer)) / 2;
    std::cout << std::setw(center) << footer << '\n';
}

template <typename I, typename N>
N iota(I f, I l, N x = N(0), N s = N(1))
{
    while (f != l) {
        *f = ValueType<I>(x);
        x += s;
        ++f;
    }
    return x;
}

template <typename I>
void iota_random(I f, I l)
{
    iota(f, l, 0, 1);
    std::random_shuffle(f, l);
}

struct test_type {
    double a[3];

    test_type() = default;
    test_type(double x) : a{x, x, x} { }
};

int main()
{
    using T = test_type;
    test_rotate<T>(1024, 16 * 1024 * 1024, 0.9, iota_random<T*>);
}
