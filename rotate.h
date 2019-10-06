#ifndef ROTATE_H
#define ROTATE_H

template <typename T>
struct distance_type;

template <typename T>
struct distance_type<T*> { using type = ptrdiff_t; };

template <typename I>
using DistanceType = typename distance_type<I>::type;

template <typename T>
struct value_type;

template <typename T>
struct value_type<T*> { using type = T; };

template <typename I>
using ValueType = typename value_type<I>::type;

template <typename T>
// requires Regular(T)
void xswap(T& x, T& y)
{
    T tmp = x;
    x = y;
    y = tmp;
}

template <typename I0, typename I1>
// requires Mutable(I0) && ForwardIteratr(I0) &&
//      Mutable(I1) && ForwardIterator(I1) &&
//      ValueType(I0) == ValueType(I1)
I1 swap_ranges(I0 f0, I0 l0, I1 f1)
{
    while (f0 != l0) {
        ::xswap(*f0, *f1);
        ++f0;
        ++f1;
    }
    return f1;
}

template <typename I0, typename I1>
// requires Mutable(I0) && BidirectionalIterator(I0) &&
//      Mutable(I1) && BidirectionalIterator(I1) &&
//      ValueType(I0) == ValueType(I1)
I1 swap_ranges_backward(I0 f0, I0 l0, I1 l1)
{
    while (f0 != l0) {
        --l0;
        --l1;
        ::xswap(*l0, *l1);
    }
    return l1;
}

template <typename I>
// requires Mutable(I) && ForwardIterator(I)
void rotate_forward_step(I& f, I& m, I l)
{
    I c = m;
    do {
        ::xswap(*f, *c);
        ++f;
        ++c;
        if (f == m) m = c;
    } while (c != l);
}

template <typename I>
// requires Mutable(I) && ForwardIterator(I)
I rotate_forward(I f, I m, I l)
{
    if (m == f) return l;
    if (m == l) return f;
    rotate_forward_step(f, m, l);
    I m_prime = f;
    while (m != l) rotate_forward_step(f, m, l);
    return m_prime;
}

template <typename I>
// requires Mutable(I) && RandomAccessIterator(I)
I rotate_forward_optimized(I f, I m, I l)
{
    if (m == f) return l;
    if (m == l) return f;
    using N = DistanceType<I>;
    N k = l - m;
    N n_minus_k = m - f;
    I m_prime = f + k;
    while (true) {
        if (n_minus_k <= k) {
            f = swap_ranges(m, l, f);
            k %= n_minus_k;
            if (k == N(0)) return m_prime;
            n_minus_k -= k;
        } else {
            l = swap_ranges_backward(f, m, l);
            n_minus_k %= k;
            if (n_minus_k == N(0)) return m_prime;
            k -= n_minus_k;
        }
        m = f + n_minus_k;
    }
}

template <typename I>
// requires Mutable(I) && RandomAccessIterator(I)
void reverse_n(I f, DistanceType<I> n)
{
    using N = DistanceType<I>;
    if (n < N(2)) return;
    DistanceType<I> i(0);
    do {
        --n;
        ::xswap(*(f + i), *(f + n));
        ++i;
    } while (i < n);
}

template <typename I>
// requires Mutable(I) && RandomAccessIterator(I)
void reverse(I f, I l)
{
    reverse_n(f, l - f);
}

template <typename I>
// requires Mutable(I) && RandomAccessIterator(I)
I rotate_bidirectional(I f, I m, I l)
{
    if (m == f) return l;
    if (m == l) return f;
    reverse(f, m);
    reverse(m, l);
    reverse(f, l);
    return f + (l - m);
}

template <typename N>
// requires Integer(N)
bool count_down(N& n)
{
    if (n == N(0)) return false;
    --n;
    return true;
}

template <typename T, typename S>
// requires EuclideanSemimodule(T, S)
T gcd(T a, T b)
{
    while (true) {
        if (b == T(0)) return a;
        a = a % b;
        if (a == T(0)) return b;
        b = b % a;
    }
}

template <typename I, typename F>
// requires Mutable(I) && Transformation(F) && ValueType(I) == Domain(F)
void cycle_from(I i, F from)
{
    I start = i;
    ValueType<I> tmp = *i;
    for (I j = from(i); j != start; j = from(j)) {
        *i = *j;
        i = j;
    }
    *i = tmp;
}

template <typename I>
// requires RandomAccessIterator(I)
struct k_rotate_from_permutation_random_access {
    DistanceType<I> k;
    DistanceType<I> n_minus_k;
    I m_prime;

    k_rotate_from_permutation_random_access(I f, I m, I l) :
        k(l - m), n_minus_k(m - f), m_prime(f + (l - m)) { }

    I operator()(I x) const
    {
        if (x < m_prime) return x + n_minus_k;
        else             return x - k;
    }
};

template <typename I, typename F>
// requires Mutable(I) && RandomAccessIterator(I) &&
//      Transformation(F) && I == Domain(F)
I rotate_cycles(I f, I m, I l, F from)
{
    using N = DistanceType<I>;
    N d = gcd<N, N>(m - f, l - m);
    while (count_down(d)) cycle_from(f + d, from);
    return f + (l - m);
}

template <typename I>
// requires Mutable(I) && RandomAccessIterator(I)
I rotate_random_access(I f, I m, I l)
{
    if (m == f) return l;
    if (m == l) return f;
    k_rotate_from_permutation_random_access<I> p(f, m, l);
    return rotate_cycles(f, m, l, p);
}

#endif
