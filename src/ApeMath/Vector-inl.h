#ifndef MyMTL_Vector_inl_h
#define MyMTL_Vector_inl_h

#pragma once

//#include "Vector.h"

#include <algorithm>
#include <functional>
#include <execution>
#include <cassert>
#include <string>
#include <stdexcept>

MYMTL_NS_BEGIN


//------------------------------------------------------------------------------
// VectorN<T, N> const_iterator implementation
//------------------------------------------------------------------------------
#if _ITERATOR_DEBUG_LEVEL == 0

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::VectorN_const_iterator() noexcept
	: ptr{ nullptr }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::VectorN_const_iterator(pointer p, size_t offset) noexcept
	: ptr{ p + offset }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::reference VectorN_const_iterator<T, Size>::operator*() const noexcept {
	return *ptr;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::pointer VectorN_const_iterator<T, Size>::operator->() const noexcept {
	return ptr;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator++() noexcept {
	++ptr;
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size> VectorN_const_iterator<T, Size>::operator++(int) noexcept {
	VectorN_const_iterator tmp = *this;
	++ptr;
	return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator--() noexcept {
	--ptr;
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size> VectorN_const_iterator<T, Size>::operator--(int) noexcept {
	VectorN_const_iterator tmp = *this;
	--ptr;
	return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator+=(const difference_type offset) noexcept {
	ptr += offset;
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator-=(const difference_type offset) noexcept {
	ptr -= offset;
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::difference_type VectorN_const_iterator<T, Size>::operator-(const VectorN_const_iterator& rhs) const noexcept {
	return ptr - rhs.ptr;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::reference VectorN_const_iterator<T, Size>::operator[](const difference_type offset) const noexcept {
	return ptr[offset];
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr bool VectorN_const_iterator<T, Size>::operator==(const VectorN_const_iterator& rhs) const noexcept {
	return ptr == rhs.ptr;
}

//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr std::strong_ordering VectorN_const_iterator<T, Size>::operator<=>(const VectorN_const_iterator& rhs) const noexcept {
//	return ptr <=> rhs.ptr;
//}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::pointer VectorN_const_iterator<T, Size>::unwrapped_ptr() const noexcept {
	return ptr;
}

// Unused
//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr void VectorN_const_iterator<T, Size>::_Seek_to(pointer it) noexcept {
//	ptr = it;
//}

#else

template<typename T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::VectorN_const_iterator() noexcept
	: ptr{ nullptr }, index{ 0 }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::VectorN_const_iterator(pointer p, size_t offset) noexcept
	: ptr{ p }, index{ offset }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::reference VectorN_const_iterator<T, Size>::operator*() const noexcept {
	return *operator->();
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::pointer VectorN_const_iterator<T, Size>::operator->() const noexcept {
	_STL_VERIFY(ptr, "cannot dereference value-initialized array iterator");
	_STL_VERIFY(index < Size, "cannot dereference out of range array iterator");
	return ptr + index;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator++() noexcept {
	_STL_VERIFY(ptr, "cannot increment value-initialized array iterator");
	_STL_VERIFY(index < Size, "cannot increment array iterator past end");
	++index;
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size> VectorN_const_iterator<T, Size>::operator++(int) noexcept {
	VectorN_const_iterator tmp = *this;
	++*this;
	return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator--() noexcept {
	_STL_VERIFY(ptr, "cannot decrement value-initialized array iterator");
	_STL_VERIFY(index != 0, "cannot decrement array iterator before begin");
	--index;
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size> VectorN_const_iterator<T, Size>::operator--(int) noexcept {
	VectorN_const_iterator tmp = *this;
	--*this;
	return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator+=(const difference_type offset) noexcept {
	dbg_verify_offset(offset);
	index += static_cast<size_t>(offset);
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>& VectorN_const_iterator<T, Size>::operator-=(const difference_type offset) noexcept {
	return *this += -offset;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::difference_type VectorN_const_iterator<T, Size>::operator-(const VectorN_const_iterator& rhs) const noexcept {
	dbg_verify_ptr(rhs);
	return static_cast<difference_type>(index - rhs.index);
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::reference VectorN_const_iterator<T, Size>::operator[](const difference_type offset) const noexcept {
	return *(*this + offset);
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr bool VectorN_const_iterator<T, Size>::operator==(const VectorN_const_iterator& rhs) const noexcept {
	dbg_verify_ptr(rhs);
	return index == rhs.index;
}

//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr std::strong_ordering VectorN_const_iterator<T, Size>::operator<=>(const VectorN_const_iterator& rhs) const noexcept {
//	dbg_verify_ptr(rhs);
//	return index <=> rhs.index;
//}

//using _Prevent_inheriting_unwrap = VectorN_const_iterator;

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_const_iterator<T, Size>::pointer VectorN_const_iterator<T, Size>::unwrapped_ptr() const noexcept {
	return ptr + index;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr void VectorN_const_iterator<T, Size>::dbg_verify_with(const VectorN_const_iterator& last) const noexcept {
	// note dbg_verify_ptr check inside operator<=
	_STL_VERIFY(*this <= last, "array iterator range transposed");
}

//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr void VectorN_const_iterator<T, Size>::_Seek_to(pointer it) noexcept {
//	index = static_cast<size_t>(it - ptr);
//}
#endif


//------------------------------------------------------------------------------
// VectorN<T, N> iterator implementation
//------------------------------------------------------------------------------
template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>::VectorN_iterator() noexcept {
	// calls const_iterator default constructor
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>::VectorN_iterator(pointer p, size_t offset) noexcept
	: my_base(p, offset) {
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>::reference VectorN_iterator<T, Size>::operator*() const noexcept {
	return const_cast<reference>(my_base::operator*());
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>::pointer VectorN_iterator<T, Size>::operator->() const noexcept {
	return const_cast<pointer>(my_base::operator->());
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>& VectorN_iterator<T, Size>::operator++() noexcept {
	my_base::operator++();
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size> VectorN_iterator<T, Size>::operator++(int) noexcept {
	VectorN_iterator tmp = *this;
	this->operator++();
	return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>& VectorN_iterator<T, Size>::operator--() noexcept {
	my_base::operator--();
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size> VectorN_iterator<T, Size>::operator--(int) noexcept {
	VectorN_iterator tmp = *this;
	this->operator--();
	return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>& VectorN_iterator<T, Size>::operator+=(const difference_type offset) noexcept {
	my_base::operator+=(offset);
	return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>& VectorN_iterator<T, Size>::operator-=(const difference_type offset) noexcept {
	my_base::operator-=(offset);
	return *this;
}

//using const_iterator::operator- for iterator subtraction;

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>::reference VectorN_iterator<T, Size>::operator[](const difference_type offset) const noexcept {
	return const_cast<reference>(my_base::operator[](offset));
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr VectorN_iterator<T, Size>::pointer VectorN_iterator<T, Size>::unwrapped_ptr() const noexcept {
	return const_cast<pointer>(my_base::unwrapped_ptr());
}


//------------------------------------------------------------------------------
// Utility namespace
//------------------------------------------------------------------------------
namespace details {
	template<typename Size>
	MYMTL_INLINE constexpr void range_bound_violation_ex(Size pos, Size n) {
		using namespace std::string_literals;
		//array::at: __n (which is 6) >= _Nm (which is 6)
		throw std::out_of_range("std::out_of_range VectorN<T, N>::at: pos (which is "s + std::to_string(pos) + ") >= N(which is "s + std::to_string(n) + ")"s);
	}

	template< typename T, typename R, std::size_t N, typename Fn >
	MYMTL_INLINE constexpr auto& vector_transform(VectorN<R, N>& res, const VectorN<T, N>& v, Fn&& fn) noexcept {
		std::transform(std::execution::unseq, std::cbegin(v), std::cend(v), std::begin(res), std::forward<Fn>(fn));

		return res;
	}

	template< typename T, typename V, typename R, std::size_t N, typename Fn >
	MYMTL_INLINE constexpr auto& vector_transform(VectorN<R, N>& res, const VectorN<T, N>& v1, const VectorN<V, N>& v2, Fn&& fn) noexcept {
		std::transform(std::execution::unseq, std::cbegin(v1), std::cend(v1), std::cbegin(v2), std::begin(res), std::forward<Fn>(fn));

		return res;
	}

	template< typename T, std::size_t N, typename Predicate >
	MYMTL_INLINE constexpr auto vector_equal(const VectorN<T, N>& v1, const VectorN<T, N>& v2, Predicate&& pred) noexcept {
		return std::equal(std::execution::unseq, std::cbegin(v1), std::cend(v1), std::cbegin(v2), std::forward<Predicate>(pred));
	}

	template< typename T, std::size_t N>
	MYMTL_INLINE constexpr auto vector_equal(const VectorN<T, N>& v1, const VectorN<T, N>& v2) noexcept {
		//ADL
		using std::equal_to;
		return vector_equal(v1, v2, equal_to{});
	}
}

//------------------------------------------------------------------------------
// ND vector iterator getters
//------------------------------------------------------------------------------
template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::iterator VectorN<T, N>::begin() noexcept {
	return iterator{ v, 0 };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::iterator VectorN<T, N>::end() noexcept {
	return iterator{ v, size() };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_iterator VectorN<T, N>::begin() const noexcept {
	return const_iterator{ v, 0 };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_iterator VectorN<T, N>::end() const noexcept {
	return const_iterator{ v, size() };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::reverse_iterator VectorN<T, N>::rbegin() noexcept {
	return reverse_iterator{ end() };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::reverse_iterator VectorN<T, N>::rend() noexcept {
	return reverse_iterator{ begin() };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_reverse_iterator VectorN<T, N>::rbegin() const noexcept {
	return const_reverse_iterator{ end() };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_reverse_iterator VectorN<T, N>::rend() const noexcept {
	return const_reverse_iterator{ begin() };
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_iterator VectorN<T, N>::cbegin() const noexcept {
	return begin();
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_iterator VectorN<T, N>::cend() const noexcept {
	return end();
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_reverse_iterator VectorN<T, N>::crbegin() const noexcept {
	return rbegin();
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_reverse_iterator VectorN<T, N>::crend() const noexcept {
	return rend();
}

//------------------------------------------------------------------------------
// ND vector member function
//------------------------------------------------------------------------------
template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::size_type VectorN<T, N>::size() noexcept {
	return N;
}

template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr bool VectorN<T, N>::is_empty() const noexcept {
	return false;
}

template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::reference VectorN<T, N>::operator[] (size_type pos) noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, N> subscript");

	return v[pos];
}

template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_reference VectorN<T, N>::operator[] (size_type pos) const noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, N> subscript");

	return v[pos];
}

template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::reference VectorN<T, N>::at(size_type pos) {
	if (N <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_reference VectorN<T, N>::at(size_type pos) const {
	if (N <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::pointer VectorN<T, N>::data() noexcept {
	return v;
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N>::const_pointer VectorN<T, N>::data() const noexcept {
	return v;
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, N>::swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>) {
	using std::swap;
	swap(v, other.v);
}

template< typename T, std::size_t N >
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, N> VectorN<T, N>::operator - () const noexcept {
	VectorN r;

	//ADL
	using std::negate;
	details::vector_transform(r, *this, negate{});

	return r;
}

template< typename T, std::size_t N >
	requires NumericType<T>
template<typename V>
	requires Addable<T, V>
MYMTL_INLINE constexpr VectorN<T, N>& VectorN<T, N>::operator += (const VectorN<V, N>& rhs) noexcept(noexcept(*data() += *rhs.data())) {
	return details::vector_transform(*this, rhs, [](auto el1, auto el2) { return static_cast<value_type>(el1 + el2); }, *this);
}

template< typename T, std::size_t N >
	requires NumericType<T>
template<typename V>
	requires Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<T, N>& VectorN<T, N>::operator -= (const VectorN<V, N>& rhs) noexcept(noexcept(*data() -= *rhs.data())) {
	return details::vector_transform(*this, rhs, [](auto el1, auto el2) { return static_cast<value_type>(el1 - el2); }, *this);
}

template< typename T, std::size_t N >
	requires NumericType<T>
template< typename U >
	requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, N>& VectorN<T, N>::operator *= (U rhs) noexcept(noexcept(*data() *= rhs)) {
	return details::vector_transform(*this, [rhs](auto el) { return el * rhs; }, *this);
}

template< typename T, std::size_t N >
	requires NumericType<T>
template< typename U >
	requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, N>& VectorN<T, N>::operator /= (U rhs) noexcept(noexcept(*data() /= rhs)) {
	//const auto inv_rhs = static_cast<T>(1.0) / rhs;

	return details::vector_transform(*this, [rhs](auto el) { return el / rhs; }, *this);
}

template<typename T, std::size_t N>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, N>::range_bound_violation(size_type pos) {
	details::range_bound_violation_ex(pos, N);
}

//------------------------------------------------------------------------------
// Non member functions and operators
//------------------------------------------------------------------------------
template< typename T, std::size_t N >
	requires (N == 0) || std::swappable<T>
MYMTL_INLINE constexpr void swap(VectorN<T, N>&lhs, VectorN<T, N>&rhs) noexcept(noexcept(lhs.swap(rhs))) {
	lhs.swap(rhs);
}

template< typename T, std::size_t N >
MYMTL_INLINE constexpr bool operator == (const VectorN<T, N>& lhs, const VectorN<T, N>& rhs) noexcept {
	return details::vector_equal(lhs, rhs);
}

//bool IsEqual( const VECTOR2D& v1, const VECTOR2D& v2, gfloat Tolerance );
//bool IsNull( const VECTOR2D& v, gfloat Tolerance );

template< typename T, typename V, typename R, std::size_t N >
	requires (N > 0) && NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, N> operator + (const VectorN<T, N>& lhs, const VectorN<V, N>& rhs) noexcept(noexcept(*lhs.data() + *rhs.data())) {
	VectorN<R, N> res;

	details::vector_transform(res, lhs, rhs, [](auto el1, auto el2) { return static_cast<R>(el1) + el2; });

	return res;
}

template< typename T, typename V, typename R, std::size_t N >
	requires (N > 0) && NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, N> operator - (const VectorN<T, N>& lhs, const VectorN<V, N>& rhs) noexcept(noexcept(*lhs.data() - *rhs.data())) {
	VectorN<R, N> res;

	details::vector_transform(res, lhs, rhs, [](auto el1, auto el2) { return static_cast<R>(el1) - el2; });

	return res;
}

template< typename T, typename U, typename R, std::size_t N >
	requires (N > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, N> operator * (const VectorN<T, N>& lhs, U rhs) noexcept(noexcept(*lhs.data()* rhs)) {
	VectorN<R, N> res;

	details::vector_transform(res, lhs, [rhs](auto el) { return static_cast<R>(el) * rhs; });

	return res;
}

template< typename T, typename U, typename R, std::size_t N >
	requires (N > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, N> operator * (U lhs, const VectorN<T, N>& rhs) noexcept(noexcept(rhs* lhs)) {
	return rhs * lhs;
}

template< typename T, typename U, typename R, std::size_t N >
	requires (N > 0) && NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, N> operator / (const VectorN<T, N>& lhs, U rhs) noexcept(noexcept(*lhs.data() / rhs)) {
	VectorN<R, N> res;

	//const auto inv_rhs = static_cast<T>(1.0) / rhs;
	details::vector_transform(res, lhs, [rhs](auto el) { return static_cast<R>(el) / rhs; });

	return res;
}


//------------------------------------------------------------------------------
// VectorN<T, 0> specialisation
// 
// 0D vector iterator getters
//------------------------------------------------------------------------------
template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::iterator VectorN<T, 0>::begin() noexcept {
	return iterator{};
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::iterator VectorN<T, 0>::end() noexcept {
	return iterator{};
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_iterator VectorN<T, 0>::begin() const noexcept {
	return const_iterator{};
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_iterator VectorN<T, 0>::end() const noexcept {
	return const_iterator{};
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::reverse_iterator VectorN<T, 0>::rbegin() noexcept {
	return reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::reverse_iterator VectorN<T, 0>::rend() noexcept {
	return reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_reverse_iterator VectorN<T, 0>::rbegin() const noexcept {
	return const_reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_reverse_iterator VectorN<T, 0>::rend() const noexcept {
	return const_reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_iterator VectorN<T, 0>::cbegin() const noexcept {
	return begin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_iterator VectorN<T, 0>::cend() const noexcept {
	return end();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_reverse_iterator VectorN<T, 0>::crbegin() const noexcept {
	return rbegin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_reverse_iterator VectorN<T, 0>::crend() const noexcept {
	return rend();
}

//--------------------------------------------------------------------
// 0D vector member function implementation
//--------------------------------------------------------------------

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::size_type VectorN<T, 0>::size() noexcept {
	return 0;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr bool VectorN<T, 0>::is_empty() const noexcept {
	return true;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::reference VectorN<T, 0>::operator[] (size_type pos) noexcept {
	assert(pos < size() && "VectorN<T, 0>::operator[]: subscript is invalid");

	return *data();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_reference VectorN<T, 0>::operator[] (size_type pos) const noexcept {
	assert(pos < size() && "VectorN<T, 0>::operator[]: subscript is invalid");

	return *data();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::reference VectorN<T, 0>::at(size_type pos) {
	range_bound_violation();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_reference VectorN<T, 0>::at(size_type pos) const {
	range_bound_violation();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::pointer VectorN<T, 0>::data() noexcept {
	return nullptr;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0>::const_pointer VectorN<T, 0>::data() const noexcept {
	return nullptr;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 0>::swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>) {
	// No op
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 0> VectorN<T, 0>::operator - () const noexcept {
	// No op
	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Addable<T, V>
MYMTL_INLINE constexpr VectorN<T, 0>& VectorN<T, 0>::operator += (const VectorN<V, 0>& rhs) noexcept {
	// No op
	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<T, 0>& VectorN<T, 0>::operator -= (const VectorN<V, 0>& rhs) noexcept {
	// No op
	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 0>& VectorN<T, 0>::operator *= (U rhs) noexcept {
	// No op
	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 0>& VectorN<T, 0>::operator /= (U rhs) noexcept {
	// No op
	return *this;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 0>::range_bound_violation() {
	throw std::out_of_range("std::out_of_range VectorN<T, 0>::at: subscript is invalid");
}


//------------------------------------------------------------------------------
// 0D vector non member function implementation
//------------------------------------------------------------------------------
template<typename T>
MYMTL_INLINE constexpr bool operator == (const VectorN< T, 0 >& lhs, const VectorN< T, 0 >& rhs) noexcept {
	return true;
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, 0> operator + (const VectorN<T, 0>& lhs, const VectorN<V, 0>& rhs) noexcept {
	VectorN<R, 0> res{ lhs };
	res += rhs;

	return res;
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<R, 0> operator - (const VectorN<T, 0>& lhs, const VectorN<V, 0>& rhs) noexcept {
	VectorN<R, 0> res{ lhs };
	res -= rhs;

	return res;
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 0> operator * (const VectorN<T, 0>& lhs, U rhs) noexcept {
	VectorN<R, 0> res{ lhs };
	res *= rhs;

	return res;
}

//template<typename T, typename U, typename R>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 0> operator * (U lhs, const VectorN<T, 0>& rhs) noexcept;

template<typename T, typename U, typename R>
	requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 0> operator / (const VectorN<T, 0>& lhs, U rhs) noexcept {
	VectorN<R, 0> res{ lhs };
	res /= rhs;

	return res;
}


//------------------------------------------------------------------------------
// VectorN<T, 1> specialisation
// 
// 1D vector iterator getters
//------------------------------------------------------------------------------
template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::iterator VectorN<T, 1>::begin() noexcept {
	return iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::iterator VectorN<T, 1>::end() noexcept {
	return iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_iterator VectorN<T, 1>::begin() const noexcept {
	return const_iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_iterator VectorN<T, 1>::end() const noexcept {
	return const_iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::reverse_iterator VectorN<T, 1>::rbegin() noexcept {
	return reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::reverse_iterator VectorN<T, 1>::rend() noexcept {
	return reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_reverse_iterator VectorN<T, 1>::rbegin() const noexcept {
	return const_reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_reverse_iterator VectorN<T, 1>::rend() const noexcept {
	return const_reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_iterator VectorN<T, 1>::cbegin() const noexcept {
	return begin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_iterator VectorN<T, 1>::cend() const noexcept {
	return end();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_reverse_iterator VectorN<T, 1>::crbegin() const noexcept {
	return rbegin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_reverse_iterator VectorN<T, 1>::crend() const noexcept {
	return rend();
}

//--------------------------------------------------------------------
// 1D vector member function implementation
//--------------------------------------------------------------------

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::size_type VectorN<T, 1>::size() noexcept {
	return 1;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr bool VectorN<T, 1>::is_empty() const noexcept {
	return false;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::reference VectorN<T, 1>::operator[] (size_type pos) noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 1> subscript");

	return data()[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_reference VectorN<T, 1>::operator[] (size_type pos) const noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 1> subscript");

	return data()[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::reference VectorN<T, 1>::at(size_type pos) {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return data()[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_reference VectorN<T, 1>::at(size_type pos) const {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return data()[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::pointer VectorN<T, 1>::data() noexcept {
	return static_cast<pointer>(&x);
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1>::const_pointer VectorN<T, 1>::data() const noexcept {
	return static_cast<const_pointer>(&x);
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 1>::swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>) {
	using std::swap;
	swap(x, other.x);
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 1> VectorN<T, 1>::operator - () const noexcept {
	return { -x };
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Addable<T, V>
MYMTL_INLINE constexpr VectorN<T, 1>& VectorN<T, 1>::operator += (const VectorN<V, 1>& rhs) noexcept(noexcept(x += rhs.x)) {
	x += rhs.x;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<T, 1>& VectorN<T, 1>::operator -= (const VectorN<V, 1>& rhs) noexcept(noexcept(x -= rhs.x)) {
	x -= rhs.x;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 1>& VectorN<T, 1>::operator *= (U rhs) noexcept(noexcept(x *= rhs)) {
	x *= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 1>& VectorN<T, 1>::operator /= (U rhs) noexcept(noexcept(x /= rhs)) {
	x /= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 1>::range_bound_violation(size_type pos) {
	details::range_bound_violation_ex(pos, 1);
}


//------------------------------------------------------------------------------
// 1D vector non member function implementation
//------------------------------------------------------------------------------
template<typename T>
MYMTL_INLINE constexpr bool operator == (const VectorN< T, 1 >& lhs, const VectorN< T, 1 >& rhs) noexcept {
	return (lhs.x == rhs.x);
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, 1> operator + (const VectorN<T, 1>& lhs, const VectorN<V, 1>& rhs) noexcept(noexcept(lhs.x + rhs.x)) {
	return VectorN<R, 1>{ static_cast<R>(lhs.x) + rhs.x };
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<R, 1> operator - (const VectorN<T, 1>& lhs, const VectorN<V, 1>& rhs) noexcept(noexcept(lhs.x - rhs.x)) {
	return VectorN<R, 1>{ static_cast<R>(lhs.x) - rhs.x };
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 1> operator * (const VectorN<T, 1>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs)) {
	return VectorN<R, 1>{ static_cast<R>(lhs.x)* rhs };
}

//template<typename T, typename U, typename R>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 1> operator * (U lhs, const VectorN<T, 1>& rhs) noexcept;

template<typename T, typename U, typename R>
	requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 1> operator / (const VectorN<T, 1>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs)) {
	return VectorN<R, 1>{ static_cast<R>(lhs.x) / rhs };
}

//------------------------------------------------------------------------------
// VectorN<T, 2> specialisation
// 
// 2D vector iterator getters
//------------------------------------------------------------------------------
template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::iterator VectorN<T, 2>::begin() noexcept {
	return iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::iterator VectorN<T, 2>::end() noexcept {
	return iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_iterator VectorN<T, 2>::begin() const noexcept {
	return const_iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_iterator VectorN<T, 2>::end() const noexcept {
	return const_iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::reverse_iterator VectorN<T, 2>::rbegin() noexcept {
	return reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::reverse_iterator VectorN<T, 2>::rend() noexcept {
	return reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_reverse_iterator VectorN<T, 2>::rbegin() const noexcept {
	return const_reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_reverse_iterator VectorN<T, 2>::rend() const noexcept {
	return const_reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_iterator VectorN<T, 2>::cbegin() const noexcept {
	return begin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_iterator VectorN<T, 2>::cend() const noexcept {
	return end();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_reverse_iterator VectorN<T, 2>::crbegin() const noexcept {
	return rbegin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_reverse_iterator VectorN<T, 2>::crend() const noexcept {
	return rend();
}

//------------------------------------------------------------------------------
// 2D vector member function implementation
//------------------------------------------------------------------------------

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::size_type VectorN<T, 2>::size() noexcept {
	return 2;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr bool VectorN<T, 2>::is_empty() const noexcept {
	return false;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::reference VectorN<T, 2>::operator[] (size_type pos) noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 2> subscript");

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_reference VectorN<T, 2>::operator[] (size_type pos) const noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 2> subscript");

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::reference VectorN<T, 2>::at(size_type pos) {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_reference VectorN<T, 2>::at(size_type pos) const {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::pointer VectorN<T, 2>::data() noexcept {
	return v;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2>::const_pointer VectorN<T, 2>::data() const noexcept {
	return v;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 2>::swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>) {
	using std::swap;
	swap(x, other.x);
	swap(y, other.y);
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 2> VectorN<T, 2>::operator - () const noexcept {
	return { -x, -y };
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Addable<T, V>
MYMTL_INLINE constexpr VectorN<T, 2>& VectorN<T, 2>::operator += (const VectorN<V, 2>& rhs) noexcept(noexcept(x += rhs.x)) {
	x += rhs.x;
	y += rhs.y;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<T, 2>& VectorN<T, 2>::operator -= (const VectorN<V, 2>& rhs) noexcept(noexcept(x -= rhs.x)) {
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 2>& VectorN<T, 2>::operator *= (U rhs) noexcept(noexcept(x *= rhs)) {
	x *= rhs;
	y *= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 2>& VectorN<T, 2>::operator /= (U rhs) noexcept(noexcept(x /= rhs)) {
	//const auto inv_rhs = static_cast<T>(1.0) / rhs;

	x /= rhs;
	y /= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 2>::range_bound_violation(size_type pos) {
	details::range_bound_violation_ex(pos, 2);
}


//------------------------------------------------------------------------------
// 2D vector non member function implementation
//------------------------------------------------------------------------------
template<typename T>
MYMTL_INLINE constexpr bool operator == (const VectorN< T, 2 >& lhs, const VectorN< T, 2 >& rhs) noexcept {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, 2> operator + (const VectorN<T, 2>& lhs, const VectorN<V, 2>& rhs) noexcept(noexcept(lhs.x + rhs.x)) {
	return VectorN<R, 2>{
		static_cast<R>(lhs.x) + rhs.x,
			static_cast<R>(lhs.y) + rhs.y };
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<R, 2> operator - (const VectorN<T, 2>& lhs, const VectorN<V, 2>& rhs) noexcept(noexcept(lhs.x - rhs.x)) {
	return VectorN<R, 2>{
		static_cast<R>(lhs.x) - rhs.x,
			static_cast<R>(lhs.y) - rhs.y };
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 2> operator * (const VectorN<T, 2>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs)) {
	return VectorN<R, 2>{
		static_cast<R>(lhs.x)* rhs,
			static_cast<R>(lhs.y)* rhs };
}

//template<typename T, typename U, typename R>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 2> operator * (U lhs, const VectorN<T, 2>& rhs) noexcept;

template<typename T, typename U, typename R>
	requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 2> operator / (const VectorN<T, 2>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs)) {
	return VectorN<R, 2>{
		static_cast<R>(lhs.x) / rhs,
			static_cast<R>(lhs.y) / rhs };
}


//------------------------------------------------------------------------------
// VectorN<T, 3> specialisation
// 
// 3D vector iterator getters
//------------------------------------------------------------------------------
template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::iterator VectorN<T, 3>::begin() noexcept {
	return iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::iterator VectorN<T, 3>::end() noexcept {
	return iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_iterator VectorN<T, 3>::begin() const noexcept {
	return const_iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_iterator VectorN<T, 3>::end() const noexcept {
	return const_iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::reverse_iterator VectorN<T, 3>::rbegin() noexcept {
	return reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::reverse_iterator VectorN<T, 3>::rend() noexcept {
	return reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_reverse_iterator VectorN<T, 3>::rbegin() const noexcept {
	return const_reverse_iterator{ end() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_reverse_iterator VectorN<T, 3>::rend() const noexcept {
	return const_reverse_iterator{ begin() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_iterator VectorN<T, 3>::cbegin() const noexcept {
	return begin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_iterator VectorN<T, 3>::cend() const noexcept {
	return end();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_reverse_iterator VectorN<T, 3>::crbegin() const noexcept {
	return rbegin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_reverse_iterator VectorN<T, 3>::crend() const noexcept {
	return rend();
}

//--------------------------------------------------------------------
// 3D vector member function implementation
//--------------------------------------------------------------------

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::size_type VectorN<T, 3>::size() noexcept {
	return 3;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr bool VectorN<T, 3>::is_empty() const noexcept {
	return false;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::reference VectorN<T, 3>::operator[] (size_type pos) noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 3> subscript");

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_reference VectorN<T, 3>::operator[] (size_type pos) const noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 3> subscript");

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::reference VectorN<T, 3>::at(size_type pos) {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_reference VectorN<T, 3>::at(size_type pos) const {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::pointer VectorN<T, 3>::data() noexcept {
	return v;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3>::const_pointer VectorN<T, 3>::data() const noexcept {
	return v;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 3>::swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>) {
	using std::swap;
	swap(x, other.x);
	swap(y, other.y);
	swap(z, other.z);
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 3> VectorN<T, 3>::operator - () const noexcept {
	return { -x, -y, -z };
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Addable<T, V>
MYMTL_INLINE constexpr VectorN<T, 3>& VectorN<T, 3>::operator += (const VectorN<V, 3>& rhs) noexcept(noexcept(x += rhs.x)) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<T, 3>& VectorN<T, 3>::operator -= (const VectorN<V, 3>& rhs) noexcept(noexcept(x -= rhs.x)) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 3>& VectorN<T, 3>::operator *= (U rhs) noexcept(noexcept(x *= rhs)) {
	x *= rhs;
	y *= rhs;
	z *= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 3>& VectorN<T, 3>::operator /= (U rhs) noexcept(noexcept(x /= rhs)) {
	//const auto inv_rhs = static_cast<T>(1.0) / rhs;

	x /= rhs;
	y /= rhs;
	z /= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 3>::range_bound_violation(size_type pos) {
	details::range_bound_violation_ex(pos, 3);
}


//------------------------------------------------------------------------------
// 3D vector non member function implementation
//------------------------------------------------------------------------------
template<typename T>
MYMTL_INLINE constexpr bool operator == (const VectorN< T, 3 >& lhs, const VectorN< T, 3 >& rhs) noexcept {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, 3> operator + (const VectorN<T, 3>& lhs, const VectorN<V, 3>& rhs) noexcept(noexcept(lhs.x + rhs.x)) {
	return VectorN<R, 3>{
		static_cast<R>(lhs.x) + rhs.x,
			static_cast<R>(lhs.y) + rhs.y,
			static_cast<R>(lhs.z) + rhs.z };
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<R, 3> operator - (const VectorN<T, 3>& lhs, const VectorN<V, 3>& rhs) noexcept(noexcept(lhs.x - rhs.x)) {
	return VectorN<R, 3>{
		static_cast<R>(lhs.x) - rhs.x,
			static_cast<R>(lhs.y) - rhs.y,
			static_cast<R>(lhs.z) - rhs.z };
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 3> operator * (const VectorN<T, 3>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs)) {
	return VectorN<R, 3>{
		static_cast<R>(lhs.x)* rhs,
			static_cast<R>(lhs.y)* rhs,
			static_cast<R>(lhs.z)* rhs };
}

//template<typename T, typename U, typename R>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 3> operator * (U lhs, const VectorN<T, 3>& rhs) noexcept;

template<typename T, typename U, typename R>
	requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 3> operator / (const VectorN<T, 3>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs)) {
	return VectorN<R, 3>{
		static_cast<R>(lhs.x) / rhs,
			static_cast<R>(lhs.y) / rhs,
			static_cast<R>(lhs.z) / rhs };
}


//------------------------------------------------------------------------------
// VectorN<T, 4> specialisation
// 
// 4D vector iterator getters
//------------------------------------------------------------------------------
template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::iterator VectorN<T, 4>::begin() noexcept {
	return iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::iterator VectorN<T, 4>::end() noexcept {
	return iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_iterator VectorN<T, 4>::begin() const noexcept {
	return const_iterator{ data(), 0 };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_iterator VectorN<T, 4>::end() const noexcept {
	return const_iterator{ data(), size() };
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::reverse_iterator VectorN<T, 4>::rbegin() noexcept {
	return reverse_iterator(end());
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::reverse_iterator VectorN<T, 4>::rend() noexcept {
	return reverse_iterator(begin());
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_reverse_iterator VectorN<T, 4>::rbegin() const noexcept {
	return const_reverse_iterator(end());
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_reverse_iterator VectorN<T, 4>::rend() const noexcept {
	return const_reverse_iterator(begin());
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_iterator VectorN<T, 4>::cbegin() const noexcept {
	return begin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_iterator VectorN<T, 4>::cend() const noexcept {
	return end();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_reverse_iterator VectorN<T, 4>::crbegin() const noexcept {
	return rbegin();
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_reverse_iterator VectorN<T, 4>::crend() const noexcept {
	return rend();
}

//--------------------------------------------------------------------
// 4D vector member function implementation
//--------------------------------------------------------------------

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::size_type VectorN<T, 4>::size() noexcept {
	return 4;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr bool VectorN<T, 4>::is_empty() const noexcept {
	return false;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::reference VectorN<T, 4>::operator[] (size_type pos) noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 4> subscript");

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_reference VectorN<T, 4>::operator[] (size_type pos) const noexcept {
	assert(pos < size() && "Invalid 'pos' in VectorN<T, 4> subscript");

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::reference VectorN<T, 4>::at(size_type pos) {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_reference VectorN<T, 4>::at(size_type pos) const {
	if (size() <= pos) {
		range_bound_violation(pos);
	}

	return v[pos];
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::pointer VectorN<T, 4>::data() noexcept {
	return v;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4>::const_pointer VectorN<T, 4>::data() const noexcept {
	return v;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 4>::swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>) {
	using std::swap;
	swap(x, other.x);
	swap(y, other.y);
	swap(z, other.z);
	swap(w, other.w);
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr VectorN<T, 4> VectorN<T, 4>::operator - () const noexcept {
	return { -x, -y, -z, -w };
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Addable<T, V>
MYMTL_INLINE constexpr VectorN<T, 4>& VectorN<T, 4>::operator += (const VectorN<V, 4>& rhs) noexcept(noexcept(x += rhs.x)) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename V>
	requires Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<T, 4>& VectorN<T, 4>::operator -= (const VectorN<V, 4>& rhs) noexcept(noexcept(x -= rhs.x)) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 4>& VectorN<T, 4>::operator *= (U rhs) noexcept(noexcept(x *= rhs)) {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
template<typename U>
	requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<T, 4>& VectorN<T, 4>::operator /= (U rhs) noexcept(noexcept(x /= rhs)) {
	//const auto inv_rhs = static_cast<T>(1.0) / rhs;

	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;

	return *this;
}

template<typename T>
	requires NumericType<T>
MYMTL_INLINE constexpr void VectorN<T, 4>::range_bound_violation(size_type pos) {
	details::range_bound_violation_ex(pos, 4);
}


//------------------------------------------------------------------------------
// 4D vector non member function implementation
//------------------------------------------------------------------------------
template<typename T>
MYMTL_INLINE constexpr bool operator == (const VectorN< T, 4 >& lhs, const VectorN< T, 4 >& rhs) noexcept {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr VectorN<R, 4> operator + (const VectorN<T, 4>& lhs, const VectorN<V, 4>& rhs) noexcept(noexcept(lhs.x + rhs.x)) {
	return VectorN<R, 4>{
		static_cast<R>(lhs.x) + rhs.x,
			static_cast<R>(lhs.y) + rhs.y,
			static_cast<R>(lhs.z) + rhs.z,
			static_cast<R>(lhs.w) + rhs.w };
}

template<typename T, typename V, typename R>
	requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr VectorN<R, 4> operator - (const VectorN<T, 4>& lhs, const VectorN<V, 4>& rhs) noexcept(noexcept(lhs.x - rhs.x)) {
	return VectorN<R, 4>{
		static_cast<R>(lhs.x) - rhs.x,
			static_cast<R>(lhs.y) - rhs.y,
			static_cast<R>(lhs.z) - rhs.z,
			static_cast<R>(lhs.w) - rhs.w };
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 4> operator * (const VectorN<T, 4>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs)) {
	return VectorN<R, 4>{
		static_cast<R>(lhs.x)* rhs,
			static_cast<R>(lhs.y)* rhs,
			static_cast<R>(lhs.z)* rhs,
			static_cast<R>(lhs.w)* rhs };
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr VectorN<R, 4> operator / (const VectorN<T, 4>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs)) {
	return VectorN<R, 4>{
		static_cast<R>(lhs.x) / rhs,
			static_cast<R>(lhs.y) / rhs,
			static_cast<R>(lhs.z) / rhs,
			static_cast<R>(lhs.w) / rhs };
}



template<typename R, std::size_t M, typename T, std::size_t N>
	requires std::convertible_to<T, R>
MYMTL_INLINE constexpr VectorN<R, M> to_vector(const VectorN<T, N>& src, R remaining_elements_init) noexcept {
	VectorN<R, M> r;

	constexpr auto Sz = (std::min)(N, M);
	auto lastIter = std::transform(src.cbegin(), src.cbegin() + Sz, r.begin(), [](auto value) { return static_cast<R>(value); });

	if (M > N) {
		std::fill_n(lastIter, M - N, remaining_elements_init);
	}

	return r;
}


//------------------------------------------------------------------------------
// Computes dot/inner product
//------------------------------------------------------------------------------
template<typename T, typename U, std::size_t N, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_INLINE constexpr R vector_dot(const VectorN<T, N>& v1, const VectorN<U, N>& v2, R init) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data())) {

	for (std::remove_cv_t<decltype(v1.size())> i = 0; i < v1.size(); i++) {
		init += static_cast<R>(static_cast<R>(v1[i]) * v2[i]);
	}

	return init;
}


template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_INLINE constexpr R vector_dot(const VectorN< T, 1 >& v1, const VectorN< U, 1 >& v2, R init) noexcept(noexcept(*v1.data()** v2.data())) {
	return init + static_cast<R>(static_cast<R>(v1.x) * v2.x);
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_INLINE constexpr R vector_dot(const VectorN< T, 2 >& v1, const VectorN< U, 2 >& v2, R init) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data())) {
	return init + static_cast<R>(static_cast<R>(v1.x) * v2.x + static_cast<R>(v1.y) * v2.y);
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_INLINE constexpr R vector_dot(const VectorN< T, 3 >& v1, const VectorN< U, 3 >& v2, R init) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data())) {
	return init + static_cast<R>(static_cast<R>(v1.x) * v2.x + static_cast<R>(v1.y) * v2.y + static_cast<R>(v1.z) * v2.z);
}

template<typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_INLINE constexpr R vector_dot(const VectorN< T, 4 >& v1, const VectorN< U, 4 >& v2, R init) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data())) {
	return init + static_cast<R>(static_cast<R>(v1.x) * v2.x + static_cast<R>(v1.y) * v2.y + static_cast<R>(v1.z) * v2.z + static_cast<R>(v1.w) * v2.w);
}

//------------------------------------------------------------------------------
// Computes cross product of the given vectors
//------------------------------------------------------------------------------
template< typename T, typename U, typename R>
	requires NumericType<R>
MYMTL_INLINE constexpr VectorN<R, 1> vector_cross(const VectorN<T, 1>& v1, const VectorN<U, 1>& v2) noexcept {
	return Vector1<R>{0};
}

template< typename T, typename U, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_INLINE constexpr VectorN<R, 3> vector_cross(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data())) {
	return Vector3<R>{
		static_cast<R>(v1.y)* v2.z - static_cast<R>(v1.z) * v2.y,
			static_cast<R>(v1.z)* v2.x - static_cast<R>(v1.x) * v2.z,
			static_cast<R>(v1.x)* v2.y - static_cast<R>(v1.y) * v2.x };
}

//------------------------------------------------------------------------------
// Computes scalar triple product of the given vectors
//------------------------------------------------------------------------------
template<typename R, typename T, typename U, typename V>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& Multipliable<T, V>&& Addable<T, V>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>() + std::declval<V>()), R>
MYMTL_INLINE constexpr R vector_scalar_triple_ert(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2, const VectorN<V, 3>& v3) noexcept(noexcept(*v1.data() + *v2.data() + *v3.data()) && noexcept(*v1.data()** v2.data()** v3.data())) {
	return static_cast<R>(static_cast<R>(v1.x) * (static_cast<R>(v2.y) * v3.z - static_cast<R>(v2.z) * v3.y)
		+ static_cast<R>(v1.y) * (static_cast<R>(v2.z) * v3.x - static_cast<R>(v2.x) * v3.z)
		+ static_cast<R>(v1.z) * (static_cast<R>(v2.x) * v3.y - static_cast<R>(v2.y) * v3.x));
}

template<typename T, typename U, typename V, typename R>
	requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& Multipliable<T, V>&& Addable<T, V>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>() + std::declval<V>()), R>
MYMTL_INLINE constexpr R vector_scalar_triple(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2, const VectorN<V, 3>& v3) noexcept(noexcept(*v1.data() + *v2.data() + *v3.data()) && noexcept(*v1.data()** v2.data()** v3.data())) {
	return vector_scalar_triple_ert<R>(v1, v2, v3);
}


MYMTL_NS_END

#endif //!MyMTL_Vector_inl_h