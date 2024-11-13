#ifndef BoundingBox_h
#define BoundingBox_h

#pragma once

#include "MTLValsCore.h"

MYMTL_NS_BEGIN

//------------------------------------------------------------------------------
// 2D bounding box (rect) left, right, top, bottom
//------------------------------------------------------------------------------

template<typename T>
struct BoundingBox2 {
	using value_type = T;

	BoundingBox2() = default;
	BoundingBox2(const BoundingBox2& src) = default;
	BoundingBox2(BoundingBox2&& other) = default;

	~BoundingBox2() = default;

	BoundingBox2(value_type initLeft, value_type initRight, value_type initTop, value_type initBottom) noexcept;

	BoundingBox2& operator = (const BoundingBox2& rhs) noexcept = default;
	BoundingBox2& operator = (BoundingBox2&& rhs) noexcept = default;

	// data

	union {
		struct {
			value_type left_;
			value_type right_;
			value_type top_;
			value_type bottom_;
		} sb;
		value_type ab[2][2];
	};
};


template<typename T>
bool operator ==(const BoundingBox2< T >& v1, const BoundingBox2< T >& v2) noexcept;

template<typename T>
bool operator !=(const BoundingBox2< T >& v1, const BoundingBox2< T >& v2) noexcept;

//bool IsEqual( const Vector3D& v1, const Vector3D& v2, gfloat Tolerance );
//bool IsNull( const Vector3D& v, gfloat Tolerance );


//------------------------------------------------------------------------------
// 3D bounding box left, right, top, bottom, far, near
//------------------------------------------------------------------------------

template<typename T>
struct BoundingBox3 {
	using value_type = T;

	BoundingBox3() = default;
	BoundingBox3(const BoundingBox3& src) = default;
	BoundingBox3(BoundingBox3&& other) = default;

	~BoundingBox3() = default;

	BoundingBox3(value_type initLeft, value_type initRight, value_type initTop, 
		value_type initBottom, value_type initFar, value_type initNear) noexcept;

	BoundingBox3& operator = (const BoundingBox3& rhs) noexcept = default;
	BoundingBox3& operator = (BoundingBox3&& rhs) noexcept = default;

	// data

	union {
		struct {
			value_type left_;
			value_type right_;
			value_type top_;
			value_type bottom_;
			value_type far_;
			value_type near_;
		} sb;
		value_type ab[3][2];
	};
};


template<typename T>
bool operator ==(const BoundingBox3< T >& v1, const BoundingBox3< T >& v2) noexcept;

template<typename T>
bool operator !=(const BoundingBox3< T >& v1, const BoundingBox3< T >& v2) noexcept;

MYMTL_NS_END

#include "BoundingBox-inl.h"

#endif // BoundingBox_h
