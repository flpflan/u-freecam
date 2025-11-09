#pragma once

template <typename T>
constexpr static T Clamp(T n, T bb, T bt)
{
    if (n < bb) return bb;
    if (n > bt) return bt;
    return n;
}

template <typename T>
constexpr static T Lerp(T x, T y, T weight)
{
    return x + (y - x) * weight;
}
