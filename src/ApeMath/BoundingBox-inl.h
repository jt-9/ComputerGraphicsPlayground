#ifndef BoundingBox_inl
#define BoundingBox_inl

#pragma once

#include "MTLValsCore.h"

MYMTL_NS_BEGIN

template<typename T>
inline BoundingBox2<T>::BoundingBox2(value_type initLeft, value_type initRight, value_type initTop, value_type initBottom) noexcept 
	: sb{ initLeft, initRight, initTop, initBottom }
{}

template<typename T>
inline bool operator ==(const BoundingBox2<T>& v1, const BoundingBox2<T>& v2) noexcept {
	return (v1.sb.left_ == v2.sb.left_) && (v1.sb.right_ == v2.sb.right_) 
		&& (v1.sb.top_ == v2.sb.top_) && (v1.sb.bottom_ == v2.sb.bottom_);
}

template<typename T>
inline bool operator !=(const BoundingBox2< T >& v1, const BoundingBox2< T >& v2) noexcept {
	return !(v1 == v2);
}



template<typename T>
inline BoundingBox3<T>::BoundingBox3(value_type initLeft, value_type initRight, value_type initTop, value_type initBottom, value_type initFar, value_type initNear) noexcept 
	: sb{ initLeft, initRight, initTop, initBottom, initFar, initNear }
{}

template<typename T>
inline bool operator ==(const BoundingBox3<T>& v1, const BoundingBox3<T>& v2) noexcept {
	return (v1.sb.left_ == v2.sb.left) && (v1.sb.right_ == v2.sb.right_)
		&& (v1.sb.top_ == v2.sb.top_) && (v1.sb.bottom_ == v2.sb.bottom_)
		&& (v1.sb.far_ == v2.sb.far_) && (v1.sb.near_ == v2.sb.near_);
}

template<typename T>
inline bool operator !=(const BoundingBox3< T >& v1, const BoundingBox3< T >& v2) noexcept {
	return !(v1 == v2);
}

MYMTL_NS_END

#endif // BoundingBox_inl