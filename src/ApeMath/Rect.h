#ifndef MyMTL_Rect_h
#define MyMTL_Rect_h

#pragma once

#include "MTLValsCore.h"
//#include "Vector.h"

#include <type_traits>

MYMTL_NS_BEGIN

//------------------------------------------------------------------------------
// Class represnets generalized Rect
//------------------------------------------------------------------------------

template<typename T>
struct Rect2
{
	using value_type = T;
	
	//bool IsEmpty() const;

	MYMTL_NO_DISCARD constexpr auto width() const noexcept;
	MYMTL_NO_DISCARD constexpr auto height() const noexcept;

	constexpr void normalize() noexcept;
	
	template< typename Vector2 >
	MYMTL_NO_DISCARD constexpr Vector2 center_point() const noexcept;

	constexpr void swap(Rect2& other) noexcept(std::is_nothrow_swappable_v<T>);

public:

	value_type left;
	value_type top;
	value_type right;
	value_type bottom;
};

template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const Rect2<T>& lhs, const Rect2<T>& rhs) noexcept;

template<typename T>
MYMTL_NO_DISCARD constexpr bool operator != (const Rect2<T>& lhs, const Rect2<T>& rhs) noexcept;

MYMTL_NS_END

#include "Rect-inl.h"

#endif // MyMTL_Rect_h