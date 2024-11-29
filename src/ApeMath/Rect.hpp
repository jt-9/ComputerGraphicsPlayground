#ifndef MYMTL_RECT_HPP
#define MYMTL_RECT_HPP

#pragma once

#include "MTLValsCore.h"

#include <type_traits>
#include <cstdlib>
#include <concepts>

MYMTL_NS_BEGIN

//------------------------------------------------------------------------------
// Class represnets generalized Rect
//------------------------------------------------------------------------------

template<typename T>
struct Rect2
{
    using value_type = T;

    MYMTL_NO_DISCARD constexpr auto width() const noexcept {
        using std::abs;
        return abs(right - left);
    }

    MYMTL_NO_DISCARD constexpr auto height() const noexcept {
        using std::abs;
        return abs(bottom - top);
    }

    constexpr void swap(Rect2& other) noexcept(std::is_nothrow_swappable_v<T>) {
        using std::swap;

        swap(left, other.left);
        swap(right, other.right);
        swap(top, other.top);
        swap(bottom, other.bottom);
    }

public:
    value_type left;
    value_type top;
    value_type right;
    value_type bottom;
};

template<typename T, typename U>
    requires std::equality_comparable_with<T, U>
MYMTL_NO_DISCARD constexpr bool operator == (const Rect2<T>& lhs, const Rect2<U>& rhs) noexcept {
    return (lhs.left == rhs.left) && (lhs.top == rhs.top) && (lhs.right == rhs.right) && (lhs.bottom == rhs.bottom);
}

template<typename T, typename U>
    requires std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr Rect2<T> rect_from(const Rect2<U>& src) noexcept {
    return Rect2<T>{
        static_cast<T>(src.left),
        static_cast<T>(src.top),
        static_cast<T>(src.right),
        static_cast<T>(src.bottom)
    };
}

MYMTL_NS_END


#endif // MYMTL_RECT_HPP