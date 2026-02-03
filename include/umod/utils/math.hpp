#pragma once

namespace umod::utils::math
{
    template <typename T>
    constexpr T clamp(T n, T bb, T bt)
    {
        if (n < bb) return bb;
        if (n > bt) return bt;
        return n;
    }

    template <typename T>
    constexpr T lerp(T x, T y, T weight)
    {
        return x + (y - x) * weight;
    }

    template <typename T>
    constexpr T abs(T n)
    {
        return n >= 0 ? n : -n;
    }
}
