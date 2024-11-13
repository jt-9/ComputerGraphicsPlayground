#ifndef MyMTL_Matrix_inl
#define MyMTL_Matrix_inl

#pragma once

//#include "Matrix.h"

#include <cassert>
#include <algorithm>
#include <functional>
#include <execution>
#include <stdexcept>
#include <string>
#include <type_traits>


MYMTL_NS_BEGIN


//------------------------------------------------------------------------------
// MatrixNxM<T, N, M> const_iterator implementation
//------------------------------------------------------------------------------
#if _ITERATOR_DEBUG_LEVEL == 0

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::MatrixNxM_const_iterator() noexcept
    : ptr{ nullptr }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::MatrixNxM_const_iterator(pointer p, size_t offset) noexcept
    : ptr{ p + offset }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::reference MatrixNxM_const_iterator<T, Size>::operator*() const noexcept {
    return *ptr;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::pointer MatrixNxM_const_iterator<T, Size>::operator->() const noexcept {
    return ptr;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator++() noexcept {
    ++ptr;
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size> MatrixNxM_const_iterator<T, Size>::operator++(int) noexcept {
    MatrixNxM_const_iterator tmp = *this;
    ++ptr;
    return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator--() noexcept {
    --ptr;
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size> MatrixNxM_const_iterator<T, Size>::operator--(int) noexcept {
    MatrixNxM_const_iterator tmp = *this;
    --ptr;
    return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator+=(const difference_type offset) noexcept {
    ptr += offset;
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator-=(const difference_type offset) noexcept {
    ptr -= offset;
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::difference_type MatrixNxM_const_iterator<T, Size>::operator-(const MatrixNxM_const_iterator& rhs) const noexcept {
    return ptr - rhs.ptr;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::reference MatrixNxM_const_iterator<T, Size>::operator[](const difference_type offset) const noexcept {
    return ptr[offset];
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr bool MatrixNxM_const_iterator<T, Size>::operator==(const MatrixNxM_const_iterator& rhs) const noexcept {
    return ptr == rhs.ptr;
}

//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr std::strong_ordering MatrixNxM_const_iterator<T, Size>::operator<=>(const MatrixNxM_const_iterator& rhs) const noexcept {
//	return ptr <=> rhs.ptr;
//}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::pointer MatrixNxM_const_iterator<T, Size>::unwrapped_ptr() const noexcept {
    return ptr;
}

// Unused
//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr void MatrixNxM_const_iterator<T, Size>::_Seek_to(pointer it) noexcept {
//	ptr = it;
//}

#else

template<typename T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::MatrixNxM_const_iterator() noexcept
    : ptr{ nullptr }, index{ 0 }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::MatrixNxM_const_iterator(pointer p, size_t offset) noexcept
    : ptr{ p }, index{ offset }
{}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::reference MatrixNxM_const_iterator<T, Size>::operator*() const noexcept {
    return *operator->();
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::pointer MatrixNxM_const_iterator<T, Size>::operator->() const noexcept {
    _STL_VERIFY(ptr, "cannot dereference value-initialized array iterator");
    _STL_VERIFY(index < Size, "cannot dereference out of range array iterator");
    return ptr + index;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator++() noexcept {
    _STL_VERIFY(ptr, "cannot increment value-initialized array iterator");
    _STL_VERIFY(index < Size, "cannot increment array iterator past end");
    ++index;
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size> MatrixNxM_const_iterator<T, Size>::operator++(int) noexcept {
    MatrixNxM_const_iterator tmp = *this;
    ++*this;
    return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator--() noexcept {
    _STL_VERIFY(ptr, "cannot decrement value-initialized array iterator");
    _STL_VERIFY(index != 0, "cannot decrement array iterator before begin");
    --index;
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size> MatrixNxM_const_iterator<T, Size>::operator--(int) noexcept {
    MatrixNxM_const_iterator tmp = *this;
    --*this;
    return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator+=(const difference_type offset) noexcept {
    dbg_verify_offset(offset);
    index += static_cast<size_t>(offset);
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>& MatrixNxM_const_iterator<T, Size>::operator-=(const difference_type offset) noexcept {
    return *this += -offset;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::difference_type MatrixNxM_const_iterator<T, Size>::operator-(const MatrixNxM_const_iterator& rhs) const noexcept {
    dbg_verify_ptr(rhs);
    return static_cast<difference_type>(index - rhs.index);
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::reference MatrixNxM_const_iterator<T, Size>::operator[](const difference_type offset) const noexcept {
    return *(*this + offset);
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr bool MatrixNxM_const_iterator<T, Size>::operator==(const MatrixNxM_const_iterator& rhs) const noexcept {
    dbg_verify_ptr(rhs);
    return index == rhs.index;
}

//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr std::strong_ordering MatrixNxM_const_iterator<T, Size>::operator<=>(const MatrixNxM_const_iterator& rhs) const noexcept {
//	dbg_verify_ptr(rhs);
//	return index <=> rhs.index;
//}

//using _Prevent_inheriting_unwrap = MatrixNxM_const_iterator;

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_const_iterator<T, Size>::pointer MatrixNxM_const_iterator<T, Size>::unwrapped_ptr() const noexcept {
    return ptr + index;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr void MatrixNxM_const_iterator<T, Size>::dbg_verify_with(const MatrixNxM_const_iterator& last) const noexcept {
    // note dbg_verify_ptr check inside operator<=
    _STL_VERIFY(*this <= last, "array iterator range transposed");
}

//template<class T, std::size_t Size>
//MYMTL_INLINE constexpr void MatrixNxM_const_iterator<T, Size>::_Seek_to(pointer it) noexcept {
//	index = static_cast<size_t>(it - ptr);
//}
#endif


//------------------------------------------------------------------------------
// Matrix iterator implementation
//------------------------------------------------------------------------------
template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>::MatrixNxM_iterator() noexcept {
    // calls const_iterator default constructor
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>::MatrixNxM_iterator(pointer p, size_t offset) noexcept
    : my_base(p, offset) {
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>::reference MatrixNxM_iterator<T, Size>::operator*() const noexcept {
    return const_cast<reference>(my_base::operator*());
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>::pointer MatrixNxM_iterator<T, Size>::operator->() const noexcept {
    return const_cast<pointer>(my_base::operator->());
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>& MatrixNxM_iterator<T, Size>::operator++() noexcept {
    my_base::operator++();
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size> MatrixNxM_iterator<T, Size>::operator++(int) noexcept {
    MatrixNxM_iterator tmp = *this;
    this->operator++();
    return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>& MatrixNxM_iterator<T, Size>::operator--() noexcept {
    my_base::operator--();
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size> MatrixNxM_iterator<T, Size>::operator--(int) noexcept {
    MatrixNxM_iterator tmp = *this;
    this->operator--();
    return tmp;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>& MatrixNxM_iterator<T, Size>::operator+=(const difference_type offset) noexcept {
    my_base::operator+=(offset);
    return *this;
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>& MatrixNxM_iterator<T, Size>::operator-=(const difference_type offset) noexcept {
    my_base::operator-=(offset);
    return *this;
}

//using const_iterator::operator- for iterator subtraction;

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>::reference MatrixNxM_iterator<T, Size>::operator[](const difference_type offset) const noexcept {
    return const_cast<reference>(my_base::operator[](offset));
}

template<class T, std::size_t Size>
MYMTL_INLINE constexpr MatrixNxM_iterator<T, Size>::pointer MatrixNxM_iterator<T, Size>::unwrapped_ptr() const noexcept {
    return const_cast<pointer>(my_base::unwrapped_ptr());
}



//------------------------------------------------------------------------------
// Utility namespace
//------------------------------------------------------------------------------
namespace details {
    template<typename Size>
    MYMTL_INLINE constexpr void range_bound_violation_ex(Size row, Size col, Size N, Size M) {
        using namespace std::string_literals;
        //array::at: __n (which is 6) >= _Nm (which is 6)
        const auto error_cause = (row >= N) ? "row index (which is "s + std::to_string(row) + ") >= N(which is "s + std::to_string(N) + ")"s
            : "column index (which is "s + std::to_string(col) + ") >= M(which is "s + std::to_string(M) + ")"s;

        throw std::out_of_range("std::out_of_range MatrixNxM<T, N, M>::at: "s + error_cause);
    }

    template<typename T, typename R, std::size_t N, std::size_t M, typename Fn>
    MYMTL_INLINE constexpr auto& matrix_transform(MatrixNxM<R, N, M>& res, const MatrixNxM<T, N, M>& m, Fn&& fn) noexcept {
        std::transform(/*std::execution::unseq, */std::cbegin(m), std::cend(m), std::begin(res), std::forward<Fn>(fn));

        return res;
    }

    template<typename T, typename V, typename R, std::size_t N, std::size_t M, typename Fn>
    MYMTL_INLINE constexpr auto& matrix_transform(MatrixNxM<R, N, M>& res, const MatrixNxM<T, N, M>& m1, const MatrixNxM<V, N, M>& m2, Fn&& fn) noexcept {
        std::transform(/*std::execution::unseq, */std::cbegin(m1), std::cend(m1), std::cbegin(m2), std::begin(res), std::forward<Fn>(fn));

        return res;
    }

    template<typename T, std::size_t N, std::size_t M, typename Predicate>
    MYMTL_INLINE constexpr auto matrix_equal(const MatrixNxM<T, N, M>& m1, const MatrixNxM<T, N, M>& m2, Predicate&& pred) noexcept {
        return std::equal(/*std::execution::unseq, */std::cbegin(m1), std::cend(m1), std::cbegin(m2), std::forward<Predicate>(pred));
    }

    template<typename T, std::size_t N, std::size_t M>
    MYMTL_INLINE constexpr auto matrix_equal(const MatrixNxM<T, N, M>& m1, const MatrixNxM<T, N, M>& m2) noexcept {
        // ADL
        using std::equal_to;
        return matrix_equal(m1, m2, equal_to{});
    }

}


//------------------------------------------------------------------------------
// Matrix of fixed size implementation
//------------------------------------------------------------------------------
template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::iterator MatrixNxM<T, N, M>::begin() noexcept {
    return iterator{ m, 0 };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::iterator MatrixNxM<T, N, M>::end() noexcept {
    return iterator{ m, size() };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_iterator MatrixNxM<T, N, M>::begin() const noexcept {
    return const_iterator{ m, 0 };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_iterator MatrixNxM<T, N, M>::end() const noexcept {
    return const_iterator{ m, size() };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::reverse_iterator MatrixNxM<T, N, M>::rbegin() noexcept {
    return reverse_iterator{ end() };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::reverse_iterator MatrixNxM<T, N, M>::rend() noexcept {
    return reverse_iterator{ begin() };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_reverse_iterator MatrixNxM<T, N, M>::rbegin() const noexcept {
    return const_reverse_iterator{ end() };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_reverse_iterator MatrixNxM<T, N, M>::rend() const noexcept {
    return const_reverse_iterator{ begin() };
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_iterator MatrixNxM<T, N, M>::cbegin() const noexcept {
    return begin();
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_iterator MatrixNxM<T, N, M>::cend() const noexcept {
    return end();
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_reverse_iterator MatrixNxM<T, N, M>::crbegin() const noexcept {
    return rbegin();
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_reverse_iterator MatrixNxM<T, N, M>::crend() const noexcept {
    return rend();
}


template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::size_type MatrixNxM<T, N, M>::rows() noexcept {
    return N;
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::size_type MatrixNxM<T, N, M>::cols() noexcept {
    return M;
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::size_type MatrixNxM<T, N, M>::size() noexcept {
    return rows() * cols();
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr bool MatrixNxM<T, N, M>::is_empty() const noexcept {
    return false;
}

// access grants
template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::reference MatrixNxM<T, N, M>::operator()(size_type row, size_type col) noexcept {
    return m[row * cols() + col];
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_reference MatrixNxM<T, N, M>::operator()(size_type row, size_type col) const noexcept {
    return m[row * cols() + col];
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::reference MatrixNxM<T, N, M>::at(size_type row, size_type col) {
    if (N <= row || M <= col) {
        range_bound_violation(row, col);
    }

    return m[row * cols() + col];
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_reference MatrixNxM<T, N, M>::at(size_type row, size_type col) const {
    if (N <= row || M <= col) {
        range_bound_violation(row, col);
    }

    return m[row * cols() + col];
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::pointer MatrixNxM<T, N, M>::data() noexcept {
    return m;
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>::const_pointer MatrixNxM<T, N, M>::data() const noexcept {
    return m;
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr void MatrixNxM<T, N, M>::swap(MatrixNxM& other) noexcept(std::is_nothrow_swappable_v<T>) {
    //std::_Swap_ranges_unchecked(m, m + size(), other.m);
    using std::swap;
    swap(m, other.m);
}

// unary operators
template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M> MatrixNxM<T, N, M>::operator-(void) const noexcept {
    MatrixNxM r;

    //ADL
    using std::negate;
    details::matrix_transform(r, *this, negate{});

    return r;
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
template<typename V>
    requires Addable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>& MatrixNxM<T, N, M>::operator +=(const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*data() += *rhs.data())) {
    return details::matrix_transform(*this, rhs, [](auto el1, auto el2) { return static_cast<value_type>(el1 + el2); }, *this);
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
template<typename V>
    requires Subtractable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>& MatrixNxM<T, N, M>::operator -=(const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*data() -= *rhs.data())) {
    return details::matrix_transform(*this, rhs, [](auto el1, auto el2) { return static_cast<value_type>(el1 - el2); }, *this);
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
template<typename V> // only square matrices support multiplication with assignment
    requires (N == M) && Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>& MatrixNxM<T, N, M>::operator *=(const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*data()** rhs.data()) && noexcept(*data() + *rhs.data())) {
    *this = matrix_multiply_ert<value_type>(*this, rhs);
    return *this;
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
template<typename U>
    requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>& MatrixNxM<T, N, M>::operator *=(U rhs) noexcept(noexcept(*data() *= rhs)) {
    return details::matrix_transform(*this, [rhs](auto el) { return el * rhs; }, *this);
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
template<typename U>
    requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M>& MatrixNxM<T, N, M>::operator /=(U rhs) noexcept(noexcept(*data() /= rhs)) {
    //const auto inv_rhs = static_cast<T>(1.0) / rhs;

    return details::matrix_transform(*this, [rhs](auto el) { return el / rhs; }, *this);
}

template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
MYMTL_INLINE constexpr void MatrixNxM<T, N, M>::range_bound_violation(size_type row, size_type col) {
    details::range_bound_violation_ex(row, col, N, M);
}


//------------------------------------------------------------------------------
// Non-member operators and methods
//------------------------------------------------------------------------------
//template<typename T, typename V, typename R, std::size_t N, std::size_t M, std::size_t P>
//	requires (N > 0) && (M > 0) && (P > 0) && Addable<T, V>&& Multipliable<T, V>
//MYMTL_INLINE constexpr auto& matrix_multiply_r(MatrixNxM<R, N, P>& __restrict r, const MatrixNxM<T, N, M>& m1, const MatrixNxM<V, M, P>& m2) noexcept(noexcept(*m1.data()** m2.data()) && noexcept(*m1.data() + *m2.data())) {
//	return detail::matrix_multiply_ert(r, m1, m2);
//}

template<typename R, typename T, typename V, std::size_t N, std::size_t M, std::size_t P>
    requires (N > 0) && (M > 0) && (P > 0) && Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, N, P> matrix_multiply_ert(const MatrixNxM<T, N, M>& m1, const MatrixNxM<V, M, P>& m2) noexcept(noexcept(*m1.data()** m2.data()) && noexcept(*m1.data() + *m2.data())) {
    //assert(m1.cols() == m2.rows() && "m1 columns count must be equal to m2 rows");

    MatrixNxM<R, N, P> res;

    for (std::remove_cvref_t<decltype(m1.rows())> i = 0; i < m1.rows(); i++) {
        auto row_begin = std::begin(res) + i * res.cols();
        std::fill(/*std::execution::unseq, */row_begin, row_begin + res.cols(), R{});

        for (std::remove_cvref_t<decltype(m2.rows())> k = 0; k < m2.rows(); k++) {
            for (std::remove_cvref_t<decltype(m2.cols())> j = 0; j < m2.cols(); j++) {
                res(i, j) += static_cast<R>(m1(i, k)) * m2(k, j);
                //res.m[i * res.cols() + j] = static_cast<R>(m1.m[i * m1.cols() + k] * m2.m[k * m2.cols() + j]);
            }
        }
    }

    return res;
}

template<typename T, std::size_t N, std::size_t M>
    requires std::swappable<T>
MYMTL_INLINE constexpr void swap(MatrixNxM<T, N, M>& lhs, MatrixNxM<T, N, M>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}


template<typename T, std::size_t N, std::size_t M>
MYMTL_INLINE constexpr bool operator == (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<T, N, M>& rhs) noexcept {
    return details::matrix_equal(lhs, rhs);
}

template<typename T, typename V, typename R, std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, N, M> operator + (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*lhs.data() + *rhs.data())) {
    MatrixNxM<R, N, M> res;

    details::matrix_transform(res, lhs, rhs, [](auto el1, auto el2) { return static_cast<decltype(res)::value_type>(el1) + el2; });

    return res;
}

template<typename T, typename V, typename R, std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, N, M> operator - (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*lhs.data() - *rhs.data())) {
    MatrixNxM<R, N, M> res;

    details::matrix_transform(res, lhs, rhs, [](auto el1, auto el2) { return static_cast<decltype(res)::value_type>(el1) - el2; });

    return res;
}

template<typename T, typename V, typename R, std::size_t N, std::size_t M, std::size_t P>
    requires (N > 0) && (M > 0) && (P > 0) && NumericType<R>&& Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, N, P> operator * (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<V, M, P>& rhs) noexcept(noexcept(*lhs.data()** rhs.data()) && noexcept(*lhs.data() + *rhs.data())) {
    return matrix_multiply_ert<R>(lhs, rhs);
}

template<typename T, typename U, typename R, std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<R, N, M> operator * (const MatrixNxM<T, N, M>& lhs, U rhs) noexcept(noexcept(*lhs.data()* rhs)) {
    MatrixNxM<R, N, M> res;

    details::matrix_transform(res, lhs, [val = rhs](auto el) { return static_cast<decltype(res)::value_type>(el) * val; });

    return res;
}

template<typename T, typename U, typename R, std::size_t N, std::size_t M>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<R, N, M> operator * (U lhs, const MatrixNxM<T, N, M>& rhs) noexcept(noexcept(rhs* lhs)) {
    return rhs * lhs;
}

template<typename T, typename U, typename R, std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<R, N, M> operator / (const MatrixNxM<T, N, M>& lhs, U rhs) noexcept(noexcept(*lhs.data() / rhs)) {
    MatrixNxM<R, N, M> res;

    //const auto inv_rhs = static_cast<R>(1.0) / rhs;

    details::matrix_transform(res, lhs, [rhs](auto el) { return static_cast<decltype(res)::value_type>(el) / rhs; });

    return res;
}

template<typename T, std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0)
MYMTL_INLINE constexpr MatrixNxM<T, M, N> matrix_transpose(const MatrixNxM<T, N, M>& m) noexcept(noexcept(std::is_nothrow_assignable_v<T, T>)) {
    MatrixNxM<T, M, N> res;
    
    for (std::remove_cvref_t<decltype(m.rows())> i = 0; i < m.rows(); i++) {
        for (std::remove_cvref_t<decltype(m.cols())> j = 0; j < m.cols(); j++) {
            res(j, i) = m(i, j);
        }
    }

    return res;
}

template<typename T, std::size_t N>
    requires (N > 0)
MYMTL_INLINE constexpr MatrixNxM<T, N, N>& matrix_transpose(MatrixNxM<T, N, N>& m) noexcept(noexcept(std::swappable<T>)) {
    using std::swap;

    for (std::remove_cvref_t<decltype(m.rows())> i = 0; i < m.rows()-1; i++) {
        for (std::remove_cvref_t<decltype(m.cols())> j = i+1; j < m.cols(); j++) {
            swap(m(i, j), m(j, i));
        }
    }

    return m;
}

//------------------------------------------------------------------------------
// MatrixNxM<T, 0, 0> specialisation
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::iterator MatrixNxM<T, 0, 0>::begin() noexcept {
    return iterator{};
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::iterator MatrixNxM<T, 0, 0>::end() noexcept {
    return iterator{};
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_iterator MatrixNxM<T, 0, 0>::begin() const noexcept {
    return const_iterator{};
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_iterator MatrixNxM<T, 0, 0>::end() const noexcept {
    return const_iterator{};
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::reverse_iterator MatrixNxM<T, 0, 0>::rbegin() noexcept {
    return reverse_iterator{ end() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::reverse_iterator MatrixNxM<T, 0, 0>::rend() noexcept {
    return reverse_iterator{ begin() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_reverse_iterator MatrixNxM<T, 0, 0>::rbegin() const noexcept {
    return const_reverse_iterator{ end() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_reverse_iterator MatrixNxM<T, 0, 0>::rend() const noexcept {
    return const_reverse_iterator{ begin() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_iterator MatrixNxM<T, 0, 0>::cbegin() const noexcept {
    return begin();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_iterator MatrixNxM<T, 0, 0>::cend() const noexcept {
    return end();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_reverse_iterator MatrixNxM<T, 0, 0>::crbegin() const noexcept {
    return rbegin();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_reverse_iterator MatrixNxM<T, 0, 0>::crend() const noexcept {
    return rend();
}


template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::size_type MatrixNxM<T, 0, 0>::rows() noexcept
{
    return 0;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::size_type MatrixNxM<T, 0, 0>::cols() noexcept {
    return 0;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::size_type MatrixNxM<T, 0, 0>::size() noexcept {
    return rows() * cols();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr bool MatrixNxM<T, 0, 0>::is_empty() const noexcept {
    return true;
}

// access grants
template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::reference MatrixNxM<T, 0, 0>::operator()(size_type row, size_type col) noexcept {
#if _CONTAINER_DEBUG_LEVEL > 0
    _STL_REPORT_ERROR("matrix row or col subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

    return *data();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_reference MatrixNxM<T, 0, 0>::operator()(size_type row, size_type col) const noexcept {
#if _CONTAINER_DEBUG_LEVEL > 0
    _STL_REPORT_ERROR("matrix row or col subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

    return *data();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::reference MatrixNxM<T, 0, 0>::at(size_type row, size_type col) {
    range_bound_violation();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_reference MatrixNxM<T, 0, 0>::at(size_type row, size_type col) const {
    range_bound_violation();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::pointer MatrixNxM<T, 0, 0>::data() noexcept {
    return nullptr;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>::const_pointer MatrixNxM<T, 0, 0>::data() const noexcept {
    return nullptr;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr void MatrixNxM<T, 0, 0>::swap(MatrixNxM& other) noexcept {
    // No op
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0> MatrixNxM<T, 0, 0>::operator-() const noexcept {
    return *this;
}

// arithmetic operators
template<typename T>
    requires NumericType<T>
template<typename V>
    requires Addable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>& MatrixNxM<T, 0, 0>::operator += (const MatrixNxM<V, 0, 0>& rhs) noexcept {
    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename V>
    requires Subtractable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>& MatrixNxM<T, 0, 0>::operator -= (const MatrixNxM<V, 0, 0>& rhs) noexcept {
    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename V>
    requires Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>& MatrixNxM<T, 0, 0>::operator *= (const MatrixNxM<V, 0, 0>& rhs) noexcept {
    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename U>
    requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>& MatrixNxM<T, 0, 0>::operator *= (U rhs) noexcept {
    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename U>
    requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0>& MatrixNxM<T, 0, 0>::operator /= (U rhs) noexcept {
    return *this;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr void MatrixNxM<T, 0, 0>::range_bound_violation() {
    throw std::out_of_range("std::out_of_range MatrixNxM<T, 0, 0>::at: subscript is invalid");
}


//------------------------------------------------------------------------------
// Non member functions and operators
//------------------------------------------------------------------------------
template<typename T, typename V, typename R>
MYMTL_INLINE constexpr MatrixNxM<R, 0, 0> matrix_multiply_ert(const MatrixNxM<T, 0, 0>& m1, const MatrixNxM<V, 0, 0>& m2) noexcept {
    return MatrixNxM<R, 0, 0>{};
}

template<typename T>
MYMTL_INLINE constexpr bool operator == (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<T, 0, 0>& rhs) noexcept;

template<typename T, typename V, typename R>
    requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, 0, 0> operator + (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<V, 0, 0>& rhs) noexcept {
    MatrixNxM<R, 0, 0> res{ lhs };

    return res += rhs;
}

template<typename T, typename V, typename R>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, 0, 0> operator - (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<V, 0, 0>& rhs) noexcept {
    MatrixNxM<R, 0, 0> res{ lhs };
    return res -= rhs;
}

template<typename T, typename V, typename R>
    requires NumericType<R>&& Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, 0, 0> operator * (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<V, 0, 0>& rhs) noexcept {
    MatrixNxM<R, 0, 0> res{ lhs };
    return res *= rhs;
}

template<typename T, typename U, typename R>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<R, 0, 0> operator * (const MatrixNxM<T, 0, 0>& lhs, U rhs) noexcept {
    MatrixNxM<R, 0, 0> res{ lhs };
    return res *= rhs;
}

template<typename T, typename U, typename R>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<R, 0, 0> operator / (const MatrixNxM<T, 0, 0>& lhs, U rhs) noexcept {
    MatrixNxM<R, 0, 0> res{ lhs };
    return res /= rhs;
}


template<typename T>
MYMTL_INLINE constexpr MatrixNxM<T, 0, 0> matrix_transpose(MatrixNxM<T, 0, 0> m) noexcept {
    return MatrixNxM<T, 0, 0>{};
}


//------------------------------------------------------------------------------
// MatrixNxM<T, 1, 1> specialisation
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::iterator MatrixNxM<T, 1, 1>::begin() noexcept {
    return iterator{ data(), 0 };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::iterator MatrixNxM<T, 1, 1>::end() noexcept {
    return iterator{ data(), size() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_iterator MatrixNxM<T, 1, 1>::begin() const noexcept {
    return const_iterator{ data(), 0 };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_iterator MatrixNxM<T, 1, 1>::end() const noexcept {
    return const_iterator{ data(), size() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::reverse_iterator MatrixNxM<T, 1, 1>::rbegin() noexcept {
    return reverse_iterator{ end() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::reverse_iterator MatrixNxM<T, 1, 1>::rend() noexcept {
    return reverse_iterator{ begin() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_reverse_iterator MatrixNxM<T, 1, 1>::rbegin() const noexcept {
    return const_reverse_iterator{ end() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_reverse_iterator MatrixNxM<T, 1, 1>::rend() const noexcept {
    return const_reverse_iterator{ begin() };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_iterator MatrixNxM<T, 1, 1>::cbegin() const noexcept {
    return begin();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_iterator MatrixNxM<T, 1, 1>::cend() const noexcept {
    return end();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_reverse_iterator MatrixNxM<T, 1, 1>::crbegin() const noexcept {
    return rbegin();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_reverse_iterator MatrixNxM<T, 1, 1>::crend() const noexcept {
    return rend();
}


template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::size_type MatrixNxM<T, 1, 1>::rows() noexcept {
    return 1;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::size_type MatrixNxM<T, 1, 1>::cols() noexcept {
    return 1;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::size_type MatrixNxM<T, 1, 1>::size() noexcept {
    return rows() * cols();
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr bool MatrixNxM<T, 1, 1>::is_empty() const noexcept {
    return true;
}

// access grants
template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::reference MatrixNxM<T, 1, 1>::operator()(size_type row, size_type col) noexcept {
    return data()[row * cols() + col];
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_reference MatrixNxM<T, 1, 1>::operator()(size_type row, size_type col) const noexcept {
    return data()[row * cols() + col];
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::reference MatrixNxM<T, 1, 1>::at(size_type row, size_type col) {
    if (rows() <= row || cols() <= col) {
        range_bound_violation(row, col);
    }

    return data()[row * cols() + col];
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_reference MatrixNxM<T, 1, 1>::at(size_type row, size_type col) const {
    if (rows() <= row || cols() <= col) {
        range_bound_violation(row, col);
    }

    return data()[row * cols() + col];
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::pointer MatrixNxM<T, 1, 1>::data() noexcept {
    return &m11;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>::const_pointer MatrixNxM<T, 1, 1>::data() const noexcept {
    return &m11;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr void MatrixNxM<T, 1, 1>::swap(MatrixNxM& other) noexcept(std::is_nothrow_swappable_v<T>) {
    //std::_Swap_ranges_unchecked(m, m + size(), other.m);
    using std::swap;
    swap(m11, other.m11);
}

// unary operators
template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1> MatrixNxM<T, 1, 1>::operator-() const noexcept {
    return MatrixNxM{ -m11 };
}

template<typename T>
    requires NumericType<T>
template<typename V>
    requires Addable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>& MatrixNxM<T, 1, 1>::operator +=(const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*data() += *rhs.data())) {
    m11 += rhs.m11;

    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename V>
    requires Subtractable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>& MatrixNxM<T, 1, 1>::operator -=(const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*data() -= *rhs.data())) {
    m11 -= rhs.m11;

    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename V>
    requires Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>& MatrixNxM<T, 1, 1>::operator *=(const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*data() *= *rhs.data())) {
    m11 *= rhs.m11;

    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename U>
    requires Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>& MatrixNxM<T, 1, 1>::operator *=(U rhs) noexcept(noexcept(*data() *= rhs)) {
    m11 *= rhs;

    return *this;
}

template<typename T>
    requires NumericType<T>
template<typename U>
    requires Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>& MatrixNxM<T, 1, 1>::operator /=(U rhs) noexcept(noexcept(*data() /= rhs)) {
    m11 /= rhs;

    return *this;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr void MatrixNxM<T, 1, 1>::range_bound_violation(size_type row, size_type col) {
    details::range_bound_violation_ex(row, col, 1, 1);
}


//------------------------------------------------------------------------------
// Non-member operators and methods
//------------------------------------------------------------------------------
template<typename R, typename T, typename V>
constexpr MatrixNxM<R, 1, 1> matrix_multiply_ert(const MatrixNxM<T, 1, 1>& m1, const MatrixNxM<V, 1, 1>& m2) noexcept(noexcept(*m1.data()** m2.data())) {
    return MatrixNxM<R, 1, 1>{ static_cast<R>(static_cast<R>(m1.m11) * m2.m11) };
}


template<typename T>
    requires std::swappable<T>
MYMTL_INLINE constexpr void swap(MatrixNxM<T, 1, 1>& lhs, MatrixNxM<T, 1, 1>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}


template<typename T>
MYMTL_INLINE constexpr bool operator == (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<T, 1, 1>& rhs) noexcept {
    return lhs.m11 == rhs.m11;
}

template<typename T, typename V, typename R>
    requires NumericType<R>&& Addable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, 1, 1> operator + (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*lhs.data() + *rhs.data())) {
    return MatrixNxM<R, 1, 1>{ lhs.m11 + rhs.m11 };
}

template<typename T, typename V, typename R>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, 1, 1> operator - (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*lhs.data() - *rhs.data())) {
    return MatrixNxM<R, 1, 1>{ lhs.m11 - rhs.m11 };
}

template<typename T, typename V, typename R>
    requires NumericType<R>&& Addable<T, V>&& Multipliable<T, V>
MYMTL_INLINE constexpr MatrixNxM<R, 1, 1> operator * (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*lhs.data()** rhs.data())) {
    return MatrixNxM<R, 1, 1>{ static_cast<R>(lhs.m11)* rhs.m11 };
}

template<typename T, typename U, typename R>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T> && (!std::same_as<U, MatrixNxM<T, 1, 1>>)
MYMTL_INLINE constexpr MatrixNxM<R, 1, 1> operator * (const MatrixNxM<T, 1, 1>& lhs, U rhs) noexcept(noexcept(*lhs.data()* rhs)) {
    return MatrixNxM<R, 1, 1>{ lhs.m11* rhs };
}

template<typename T, typename U, typename R>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_INLINE constexpr MatrixNxM<R, 1, 1> operator / (const MatrixNxM<T, 1, 1>& lhs, U rhs) noexcept(noexcept(*lhs.data() / rhs)) {
    return MatrixNxM<R, 1, 1>{ lhs.m11 / rhs };
}

template<typename T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1> matrix_transpose(const MatrixNxM<T, 1, 1>& m) noexcept(noexcept(std::is_nothrow_assignable_v<T, T>)) {
    return MatrixNxM<T, 1, 1>{ m.m11 };
}

template<typename T>
MYMTL_INLINE constexpr MatrixNxM<T, 1, 1>& matrix_transpose(MatrixNxM<T, 1, 1>& m) noexcept {
    return m;
}

//	//***************************************************************************************
//	//---------------------------------------------------------------------------------------
//	// returns the matrix with opposite numbers
//	//---------------------------------------------------------------------------------------
//	template<typename T>
//	MYMTL_INLINE constexpr Matrix3< T > Matrix3< T >::operator - (void) const noexcept
//	{
//		return Matrix3<T>{
//			-am._11, -am._12, -am._13,
//				-am._21, -am._22, -am._23,
//				-am._31, -am._32, -am._33};
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr Matrix3< T >& Matrix3< T >::operator += (const Matrix3& rhs) noexcept
//	{
//		am._11 += rhs.am._11; am._12 += rhs.am._12; am._13 += rhs.am._13;
//		am._21 += rhs.am._21; am._22 += rhs.am._22; am._23 += rhs.am._23;
//		am._31 += rhs.am._31; am._32 += rhs.am._32; am._33 += rhs.am._33;
//
//		return *this;
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr Matrix3< T >& Matrix3< T >::operator -= (const Matrix3& rhs) noexcept
//	{
//		am._11 -= rhs.am._11; am._12 -= rhs.am._12; am._13 -= rhs.am._13;
//		am._21 -= rhs.am._21; am._22 -= rhs.am._22; am._23 -= rhs.am._23;
//		am._31 -= rhs.am._31; am._32 -= rhs.am._32; am._33 -= rhs.am._33;
//
//		return *this;
//	}
//
//
//	template<typename T>
//	template<typename U>
//	MYMTL_INLINE constexpr Matrix3< T >& Matrix3< T >::operator *= (const U val) noexcept
//	{
//		am._11 *= val; am._12 *= val; am._13 *= val;
//		am._21 *= val; am._22 *= val; am._23 *= val;
//		am._31 *= val; am._32 *= val; am._33 *= val;
//
//		return *this;
//	}
//
//	template<typename T>
//	template<typename U>
//	MYMTL_INLINE constexpr Matrix3< T >& Matrix3< T >::operator /= (const U val) noexcept
//	{
//		const value_type inv_val = static_cast<value_type>(1.0) / val;
//		am._11 *= inv_val; am._12 *= inv_val; am._13 *= inv_val;
//		am._21 *= inv_val; am._22 *= inv_val; am._23 *= inv_val;
//		am._31 *= inv_val; am._32 *= inv_val; am._33 *= inv_val;
//
//		return *this;
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr auto& matrix_multiply_ert(Matrix3<T>& res, const Matrix3<T>& m1, const Matrix3<T>& m2) noexcept
//	{
//		//#pragma omp parallel sections
//		//{
//		//#pragma omp section
//		//{
//		res.am._11 = m1.am._11*m2.am._11 + m1.am._12*m2.am._21 + m1.am._13*m2.am._31;
//		res.am._12 = m1.am._11*m2.am._12 + m1.am._12*m2.am._22 + m1.am._13*m2.am._32;
//		res.am._13 = m1.am._11*m2.am._13 + m1.am._12*m2.am._23 + m1.am._13*m2.am._33;
//		//}
//		//	
//		//#pragma omp section
//		//{
//		res.am._21 = m1.am._21*m2.am._11 + m1.am._22*m2.am._21 + m1.am._23*m2.am._31;
//		res.am._22 = m1.am._21*m2.am._12 + m1.am._22*m2.am._22 + m1.am._23*m2.am._32;
//		res.am._23 = m1.am._21*m2.am._13 + m1.am._22*m2.am._23 + m1.am._23*m2.am._33;
//		//}
//
//		//#pragma omp section
//		//{
//		res.am._31 = m1.am._31*m2.am._11 + m1.am._32*m2.am._21 + m1.am._33*m2.am._31;
//		res.am._32 = m1.am._31*m2.am._12 + m1.am._32*m2.am._22 + m1.am._33*m2.am._32;
//		res.am._33 = m1.am._31*m2.am._13 + m1.am._32*m2.am._23 + m1.am._33*m2.am._33;
//		//}
//		//}
//		return res;
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr bool operator == (const Matrix3<T>& m1, const Matrix3<T>& m2) noexcept
//	{
//		//return ( 0 == ::memcmp( &m1, &m2, sizeof( Matrix3 ) ) );
//		return ((m1.am._11 == m2.am._11) && (m1.am._12 == m2.am._12) && (m1.am._13 == m2.am._13)
//			&& (m1.am._21 == m2.am._21) && (m1.am._22 == m2.am._22) && (m1.am._23 == m2.am._23)
//			&& (m1.am._31 == m2.am._31) && (m1.am._32 == m2.am._32) && (m1.am._33 == m2.am._33));
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr bool operator != (const Matrix3<T>& m1, const Matrix3<T>& m2) noexcept
//	{
//		return !(m1 == m2);
//		//return ( 0 != ::memcmp( &m1, &m2, sizeof( Matrix3 ) ) );
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr Matrix3<T> operator * (const Matrix3<T>& lhs, const Matrix3<T>& rhs) noexcept
//	{
//		Matrix3< T > res;
//		return matrix_multiply_ert(res, lhs, rhs);
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr Matrix3<T> operator + (const Matrix3<T>& lhs, const Matrix3<T>& rhs) noexcept
//	{
//		return Matrix3(lhs.am._11 + rhs.am._11, lhs.am._12 + rhs.am._12, lhs.am._13 + rhs.am._13,
//			lhs.am._21 + rhs.am._21, lhs.am._22 + rhs.am._22, lhs.am._23 + rhs.am._23,
//			lhs.am._31 + rhs.am._31, lhs.am._32 + rhs.am._32, lhs.am._33 + rhs.am._33);
//	}
//
//	template<typename T>
//	MYMTL_INLINE constexpr Matrix3<T> operator - (const Matrix3<T>& lhs, const Matrix3<T>& rhs) noexcept
//	{
//		return Matrix3(lhs.am._11 - rhs.am._11, lhs.am._12 - rhs.am._12, lhs.am._13 - rhs.am._13,
//			lhs.am._21 - rhs.am._21, lhs.am._22 - rhs.am._22, lhs.am._23 - rhs.am._23,
//			lhs.am._31 - rhs.am._31, lhs.am._32 - rhs.am._32, lhs.am._33 - rhs.am._33);
//	}

    //mymtl_inline Matrix3 operator * ( const Matrix3& m, gfloat value )
    //{
    //	return Matrix3( m._11 * value, m._12 * value, m._13 * value,
    //		m._21 * value, m._22 * value, m._23 * value,
    //		m._31 * value, m._32 * value, m._33 * value );
    //}
    //
    //mymtl_inline Matrix3 operator * ( gfloat value, const Matrix3& m )
    //{
    //	return Matrix3( value * m._11, value * m._12, value * m._13,
    //		value * m._21, value * m._22, value * m._23, 
    //		value * m._31, value * m._32, value * m._33 );
    //}
    //
    //mymtl_inline Matrix3 operator / ( const Matrix3& m, gfloat value )
    //{
    //	gfloat valueInv = ( gfloat )1.0 / value;
    //	return Matrix3( m._11 * valueInv, m._12 * valueInv, m._13 * valueInv,
    //		m._21 * valueInv, m._22 * valueInv, m._23 * valueInv,
    //		m._31 * valueInv, m._32 * valueInv, m._33 * valueInv );
    //}
    //
    //


    //------------------------------------------------------------------------------
    // Squared matrix 4 dimensional
    //------------------------------------------------------------------------------
    /*
    #ifdef _DEBUG
    template <typename T>
    mymtl_inline Matrix4::Matrix4( const T* pm )
    {
        if ( !pm )
            return;

        _11 = pm[0]; _12 = pm[1]; _13 = pm[2];	_14 = pm[3];
        _21 = pm[4]; _22 = pm[5]; _23 = pm[6];	_24 = pm[7];
        _31 = pm[8]; _32 = pm[9]; _33 = pm[10]; _34 = pm[11];
        _41 = pm[12]; _42 = pm[13]; _43 = pm[14]; _44 = pm[15];
        //::memcpy( &_11, pm, sizeof( Matrix3 ) );
    }
    #else
    template <typename T>
    mymtl_inline Matrix4::Matrix4( const T* pm )
        : _11( pm[0] ), _12( pm[1] ), _13( pm[2] )
        , _21( pm[3] ), _22( pm[4] ), _23( pm[5] )
        , _31( pm[6] ), _32( pm[7] ), _33( pm[8] )
    {}
    #endif

    mymtl_inline Matrix4::Matrix4( const Matrix4& src )
    {
        ::memcpy( &_11, &src, sizeof( Matrix4 ) );
    }

    mymtl_inline Matrix4::Matrix4( gfloat el11, gfloat el12, gfloat el13, gfloat el14,
                                 gfloat el21, gfloat el22, gfloat el23, gfloat el24,
                                 gfloat el31, gfloat el32, gfloat el33, gfloat el34,
                                 gfloat el41, gfloat el42, gfloat el43, gfloat el44 )
    {
        _11 = el11; _12 = el12; _13 = el13; _14 = el14;
        _21 = el21; _22 = el22; _23 = el23; _24 = el24;
        _31 = el31; _32 = el32; _33 = el33; _34 = el34;
        _41 = el41; _42 = el42; _43 = el43; _44 = el44;
    }

    mymtl_inline gfloat* Matrix4::operator [] ( unsigned char index )
    {
        return *( m + index );
    }

    mymtl_inline const gfloat* Matrix4::operator [] ( unsigned char index ) const
    {
        return *( m + index );
    }

    mymtl_inline Matrix4& Matrix4::operator = ( const Matrix4& rhs )
    {
        if ( &rhs != this )
        {
            ::memcpy( &_11, &rhs._11, sizeof( Matrix4 ) );
        }
        return *this;
    }

    mymtl_inline Matrix4& Matrix4::operator *= ( const Matrix4& rhs )
    {
        Matrix4 lhs( *this );
        return matrix_multiply_ert( *this, lhs, rhs );
    }

    mymtl_inline Matrix4& Matrix4::operator += ( const Matrix4& rhs )
    {
        _11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
        _21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
        _31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
        _41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;

        return *this;
    }

    mymtl_inline Matrix4& Matrix4::operator -= ( const Matrix4& rhs )
    {
        _11 -= rhs._11; _12 -= rhs._12; _13 -= rhs._13; _14 -= rhs._14;
        _21 -= rhs._21; _22 -= rhs._22; _23 -= rhs._23; _24 -= rhs._24;
        _31 -= rhs._31; _32 -= rhs._32; _33 -= rhs._33; _34 -= rhs._34;
        _41 -= rhs._41; _42 -= rhs._42; _43 -= rhs._43; _44 -= rhs._44;

        return *this;
    }

    mymtl_inline Matrix4& Matrix4::operator *= ( gfloat value )
    {
        _11 *= value; _12 *= value; _13 *= value; _14 *= value;
        _21 *= value; _22 *= value; _23 *= value; _24 *= value;
        _31 *= value; _32 *= value; _33 *= value; _34 *= value;
        _41 *= value; _42 *= value; _43 *= value; _44 *= value;

        return *this;
    }

    mymtl_inline Matrix4& Matrix4::operator /= ( gfloat value )
    {
        gfloat valueInv = ( gfloat )1.0 / value;
        _11 *= valueInv; _12 *= valueInv; _13 *= valueInv; _14 *= valueInv;
        _21 *= valueInv; _22 *= valueInv; _23 *= valueInv; _24 *= valueInv;
        _31 *= valueInv; _32 *= valueInv; _33 *= valueInv; _34 *= valueInv;
        _41 *= valueInv; _42 *= valueInv; _43 *= valueInv; _44 *= valueInv;

        return *this;
    }

    mymtl_inline Matrix4 Matrix4::operator - () const
    {
        return Matrix4(	-_11, -_12, -_13, -_14,
                            -_21, -_22, -_23, -_24,
                            -_31, -_32, -_33, -_34,
                            -_41, -_42, -_43, -_44 );
    }

    mymtl_inline bool operator == ( const Matrix4& m1, const Matrix4& m2 )
    {
        return ( 0 == ::memcmp( &m1, &m2, sizeof( Matrix4 ) ) );
    }

    mymtl_inline bool operator != ( const Matrix4& m1, const Matrix4& m2 )
    {
        return ( 0 != ::memcmp( &m1, &m2, sizeof( Matrix4 ) ) );
    }

    mymtl_inline Matrix4 operator * ( const Matrix4& lhs, const Matrix4& rhs )
    {
        Matrix4 res;
        return matrix_multiply_ert( res, lhs, rhs );
    }

    mymtl_inline Matrix4 operator + ( const Matrix4& lhs, const Matrix4& rhs )
    {
        return Matrix4( lhs._11 + rhs._11, lhs._12 + rhs._12, lhs._13 + rhs._13, lhs._14 + rhs._14,
                          lhs._21 + rhs._21, lhs._22 + rhs._22, lhs._23 + rhs._23, lhs._24 + rhs._24,
                          lhs._31 + rhs._31, lhs._32 + rhs._32, lhs._33 + rhs._33, lhs._34 + rhs._34,
                          lhs._41 + rhs._41, lhs._42 + rhs._42, lhs._43 + rhs._43, lhs._44 + rhs._44 );
    }

    mymtl_inline Matrix4 operator - ( const Matrix4& lhs, const Matrix4& rhs )
    {
        return Matrix4( lhs._11 - rhs._11, lhs._12 - rhs._12, lhs._13 - rhs._13, lhs._14 - rhs._14,
                          lhs._21 - rhs._21, lhs._22 - rhs._22, lhs._23 - rhs._23, lhs._24 - rhs._24,
                          lhs._31 - rhs._31, lhs._32 - rhs._32, lhs._33 - rhs._33, lhs._34 - rhs._34,
                          lhs._41 - rhs._41, lhs._42 - rhs._42, lhs._43 - rhs._43, lhs._44 - rhs._44 );
    }

    mymtl_inline Matrix4 operator * ( const Matrix4& m, gfloat value )
    {
        return Matrix4( m._11 * value, m._12 * value, m._13 * value, m._14 * value,
                          m._21 * value, m._22 * value, m._23 * value, m._24 * value,
                          m._31 * value, m._32 * value, m._33 * value, m._34 * value,
                          m._41 * value, m._42 * value, m._43 * value, m._44 * value );
    }

    mymtl_inline Matrix4 operator * ( gfloat value, const Matrix4& m )
    {
        return Matrix4( value * m._11, value * m._12, value * m._13, value * m._14,
                          value * m._21, value * m._22, value * m._23, value * m._24,
                          value * m._31, value * m._32, value * m._33, value * m._34,
                          value * m._41, value * m._42, value * m._43, value * m._44);
    }

    mymtl_inline Matrix4 operator / ( const Matrix4& m, gfloat value )
    {
        gfloat valueInv = ( gfloat )1.0 / value;
        return Matrix4( m._11 * valueInv, m._12 * valueInv, m._13 * valueInv, m._14 * valueInv,
                          m._21 * valueInv, m._22 * valueInv, m._23 * valueInv, m._24 * valueInv,
                          m._31 * valueInv, m._32 * valueInv, m._33 * valueInv, m._34 * valueInv,
                          m._41 * valueInv, m._42 * valueInv, m._43 * valueInv, m._44 * valueInv);
    }


    mymtl_inline Matrix4& matrix_multiply_ert( Matrix4& res, const Matrix4 &m1, const Matrix4 &m2 )
    {
        res._11 = m1._11*m2._11 + m1._12*m2._21 + m1._13*m2._31 + m1._14*m2._41;
        res._12 = m1._11*m2._12 + m1._12*m2._22 + m1._13*m2._32 + m1._14*m2._42;
        res._13 = m1._11*m2._13 + m1._12*m2._23 + m1._13*m2._33 + m1._14*m2._43;
        res._14 = m1._11*m2._14 + m1._12*m2._24 + m1._13*m2._34 + m1._14*m2._44;

        res._21 = m1._21*m2._11 + m1._22*m2._21 + m1._23*m2._31 + m1._24*m2._41;
        res._22 = m1._21*m2._12 + m1._22*m2._22 + m1._23*m2._32 + m1._24*m2._42;
        res._23 = m1._21*m2._13 + m1._22*m2._23 + m1._23*m2._33 + m1._24*m2._43;
        res._24 = m1._21*m2._14 + m1._22*m2._24 + m1._23*m2._34 + m1._24*m2._44;

        res._31 = m1._31*m2._11 + m1._32*m2._21 + m1._33*m2._31 + m1._34*m2._41;
        res._32 = m1._31*m2._12 + m1._32*m2._22 + m1._33*m2._32 + m1._34*m2._42;
        res._33 = m1._31*m2._13 + m1._32*m2._23 + m1._33*m2._33 + m1._34*m2._43;
        res._34 = m1._31*m2._14 + m1._32*m2._24 + m1._33*m2._34 + m1._34*m2._44;

        res._41 = m1._41*m2._11 + m1._42*m2._21 + m1._43*m2._31 + m1._44*m2._41;
        res._42 = m1._41*m2._12 + m1._42*m2._22 + m1._43*m2._32 + m1._44*m2._42;
        res._43 = m1._41*m2._13 + m1._42*m2._23 + m1._43*m2._33 + m1._44*m2._43;
        res._44 = m1._41*m2._14 + m1._42*m2._24 + m1._43*m2._34 + m1._44*m2._44;

        return res;
    }

    */

MYMTL_NS_END

#include "Matrix-std.h"

#endif // MyMTL_Matrix_inl

