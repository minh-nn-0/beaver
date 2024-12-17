#ifndef BEAVER_ENUMBITOPS_H
#define BEAVER_ENUMBITOPS_H

// CREDIT: from https://www.reddit.com/user/bkentel/
// Stolen with love

#include <type_traits>

template <typename T>
concept enum_class =
       std::is_enum_v<T>
    && !std::is_convertible_v<T, std::underlying_type_t<T>>;

// extension point
template <enum_class T>
inline constexpr bool enable_enum_bitops = false;

template <typename T>
concept enum_bitops = enum_class<T> && enable_enum_bitops<T>;

template <enum_bitops T>
[[nodiscard]] constexpr T operator|(T const a, T const b) noexcept
{
    using I = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<I>(a) | static_cast<I>(b));
}

template <enum_bitops T>
[[nodiscard]] constexpr T operator&(T const a, T const b) noexcept
{
    using I = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<I>(a) & static_cast<I>(b));
}

template <enum_bitops T>
constexpr T& operator|=(T& a, T const b) noexcept
{
    return a = a | b;
}

template <enum_bitops T>
constexpr T& operator&=(T& a, T const b) noexcept
{
    return a = a & b;
}

template <enum_bitops T>
[[nodiscard]] constexpr T operator~(T const a) noexcept
{
    using I = std::underlying_type_t<T>;
    return static_cast<T>(~static_cast<I>(a));
}

#endif
