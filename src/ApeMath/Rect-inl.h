#ifndef MyMTL_Rect_inl
#define MyMTL_Rect_inl

#pragma once

#include "Rect.h"
#include <utility>

MYMTL_NS_BEGIN

//------------------------------------------------------------------------------
// Class Rect2 implementation
//------------------------------------------------------------------------------

template<typename T>
MYMTL_INLINE constexpr auto Rect2< T >::width() const noexcept {
	return right - left;
}

template<typename T>
MYMTL_INLINE constexpr auto Rect2< T >::height() const noexcept {
	return bottom - top;
}

template<typename T>
template< typename Vector2 >
MYMTL_INLINE constexpr Vector2 Rect2< T >::center_point() const noexcept
{
	return Vector2{ (left + right) / 2, (top + bottom) / 2 };
}


template<typename T>
MYMTL_INLINE constexpr void Rect2< T >::normalize() noexcept
{
	if ( right < left )
	{
		std::swap(left, right);
	}

	if ( bottom < top )
	{
		std::swap(top, bottom);
	}
}

template<typename T>
MYMTL_INLINE constexpr void Rect2< T >::swap(Rect2& other) noexcept(std::is_nothrow_swappable_v<T>) {
	std::swap(left, other.left);
	std::swap(right, other.right);
	std::swap(top, other.top);
	std::swap(bottom, other.bottom);
}

template<typename T>
MYMTL_INLINE constexpr bool operator ==(const Rect2<T>& lhs, const Rect2<T>& rhs) noexcept {
	return (lhs.left == rhs.left) && (lhs.right == rhs.right) && (lhs.top == rhs.top) && (lhs.bottom == rhs.bottom);
}

template<typename T>
MYMTL_INLINE constexpr bool operator !=(const Rect2<T>& lhs, const Rect2<T>& rhs) noexcept {
	return !(*this == rhs);
}


MYMTL_NS_END

#endif // !MyMTL_Rect_inl