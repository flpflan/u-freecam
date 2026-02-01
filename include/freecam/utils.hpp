#pragma once

template <typename T>
constexpr T Clamp(T n, T bb, T bt)
{
    if (n < bb) return bb;
    if (n > bt) return bt;
    return n;
}

template <typename T>
constexpr T Lerp(T x, T y, T weight)
{
    return x + (y - x) * weight;
}

template <typename T>
constexpr T Abs(T n)
{
    return n >= 0 ? n : -n;
}
