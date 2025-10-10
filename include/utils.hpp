#pragma once

template <typename T>
constexpr inline static T Clamp(T n, T bb, T bt)
{
    if (n < bb) return bb;
    if (n > bt) return bt;
    return n;
}

