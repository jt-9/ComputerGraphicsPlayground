#ifndef MyMTL_Vector_h
#define MyMTL_Vector_h

#pragma once

#include "MTLValsCore.h"
#include "MTLConcepts.h"

#include <iterator>
#include <limits>

MYMTL_NS_BEGIN

//------------------------------------------------------------------------------
// Matrix const_iterator
//------------------------------------------------------------------------------
template <class T, std::size_t Size>
class VectorN_const_iterator {
public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

#if _ITERATOR_DEBUG_LEVEL == 0
    constexpr VectorN_const_iterator() noexcept;

    constexpr explicit VectorN_const_iterator(pointer p, size_t offset = 0) noexcept;

    MYMTL_NO_DISCARD constexpr reference operator*() const noexcept;

    MYMTL_NO_DISCARD constexpr pointer operator->() const noexcept;

    constexpr VectorN_const_iterator& operator++() noexcept;
    constexpr VectorN_const_iterator operator++(int) noexcept;

    constexpr VectorN_const_iterator& operator--() noexcept;
    constexpr VectorN_const_iterator operator--(int) noexcept;

    constexpr VectorN_const_iterator& operator+=(const difference_type offset) noexcept;
    constexpr VectorN_const_iterator& operator-=(const difference_type offset) noexcept;

    MYMTL_NO_DISCARD constexpr difference_type operator-(const VectorN_const_iterator& rhs) const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[](const difference_type offset) const noexcept;

    MYMTL_NO_DISCARD constexpr bool operator==(const VectorN_const_iterator& rhs) const noexcept;

    // Does not see implementation in inl file, at least IntelliSense undelines it as missing implementation
    MYMTL_NO_DISCARD MYMTL_INLINE constexpr std::strong_ordering operator<=>(const VectorN_const_iterator& rhs) const noexcept {
        return ptr <=> rhs.ptr;
    }

    MYMTL_NO_DISCARD constexpr pointer unwrapped_ptr() const noexcept;

    // Unused
    //constexpr void _Seek_to(pointer it) noexcept;

private:
    pointer ptr; // beginning of array
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 / _ITERATOR_DEBUG_LEVEL != 0 vvv

public:
    constexpr VectorN_const_iterator() noexcept;

    constexpr explicit VectorN_const_iterator(pointer p, size_t offset = 0) noexcept;

    MYMTL_NO_DISCARD constexpr reference operator*() const noexcept;

    MYMTL_NO_DISCARD constexpr pointer operator->() const noexcept;

    constexpr VectorN_const_iterator& operator++() noexcept;

    constexpr VectorN_const_iterator operator++(int) noexcept;

    constexpr VectorN_const_iterator& operator--() noexcept;

    constexpr VectorN_const_iterator operator--(int) noexcept;

    constexpr VectorN_const_iterator& operator+=(const difference_type offset) noexcept;

    constexpr VectorN_const_iterator& operator-=(const difference_type offset) noexcept;

    MYMTL_NO_DISCARD constexpr difference_type operator-(const VectorN_const_iterator& rhs) const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[](const difference_type offset) const noexcept;

    MYMTL_NO_DISCARD constexpr bool operator==(const VectorN_const_iterator& rhs) const noexcept;

    MYMTL_NO_DISCARD constexpr std::strong_ordering operator<=>(const VectorN_const_iterator& rhs) const noexcept {
        dbg_verify_ptr(rhs);
        return index <=> rhs.index;
    }

    MYMTL_NO_DISCARD constexpr pointer unwrapped_ptr() const noexcept;
    constexpr void dbg_verify_with(const VectorN_const_iterator& last) const noexcept;

    //Unused
    //constexpr void _Seek_to(pointer it) noexcept;

private:
    using size_type = std::size_t;

    MYMTL_INLINE constexpr void dbg_verify_offset(const difference_type offset) const noexcept {
        if (offset != 0) {
            _STL_VERIFY(ptr, "cannot seek value-initialized array iterator");
        }

        if (offset < 0) {
            _STL_VERIFY(index >= size_type{ 0 } - static_cast<size_type>(offset), "cannot seek array iterator before begin");
        }

        if (offset > 0) {
            _STL_VERIFY(Size - index >= static_cast<size_type>(offset), "cannot seek array iterator after end");
        }
    }

    MYMTL_INLINE constexpr void dbg_verify_ptr(const VectorN_const_iterator& rhs) const noexcept { // test for compatible iterator pair
        _STL_VERIFY(ptr == rhs.ptr, "array iterators incompatible");
    }

private:
    pointer ptr; // beginning of array
    size_type index; // offset into array
#endif // ^^^ _ITERATOR_DEBUG_LEVEL != 0 ^^^

public:
    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr VectorN_const_iterator operator+(VectorN_const_iterator next, const difference_type offset) noexcept {
        VectorN_const_iterator tmp = next;
        tmp += offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr VectorN_const_iterator operator-(VectorN_const_iterator next, const difference_type offset) noexcept {
        VectorN_const_iterator tmp = next;
        tmp -= offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr VectorN_const_iterator operator+(const difference_type offset, VectorN_const_iterator next) noexcept {
        next += offset;
        return next;
    }
}; //VectorN_const_iterator

//------------------------------------------------------------------------------
// Matrix iterator
//------------------------------------------------------------------------------
template <class T, std::size_t Size>
class VectorN_iterator : public VectorN_const_iterator<T, Size> {
public:
    using my_base = VectorN_const_iterator<T, Size>;

    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    constexpr VectorN_iterator() noexcept;

    constexpr explicit VectorN_iterator(pointer p, size_t offset = 0) noexcept;

    MYMTL_NO_DISCARD constexpr reference operator*() const noexcept;

    MYMTL_NO_DISCARD constexpr pointer operator->() const noexcept;

    constexpr VectorN_iterator& operator++() noexcept;
    constexpr VectorN_iterator operator++(int) noexcept;

    constexpr VectorN_iterator& operator--() noexcept;
    constexpr VectorN_iterator operator--(int) noexcept;

    constexpr VectorN_iterator& operator+=(const difference_type offset) noexcept;
    constexpr VectorN_iterator& operator-=(const difference_type offset) noexcept;

    using my_base::operator-;

    MYMTL_NO_DISCARD constexpr reference operator[](const difference_type offset) const noexcept;

    //using my_base::operator<=>;

    MYMTL_NO_DISCARD constexpr pointer unwrapped_ptr() const noexcept;

    // Unused
    //constexpr void _Seek_to(pointer it) noexcept;

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr VectorN_iterator operator+(VectorN_iterator next, const difference_type offset) noexcept {
        VectorN_iterator tmp = next;
        tmp += offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr VectorN_iterator operator-(VectorN_iterator next, const difference_type offset) noexcept {
        VectorN_iterator tmp = next;
        tmp -= offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr VectorN_iterator operator+(const difference_type offset, VectorN_iterator next) noexcept {
        next += offset;
        return next;
    }
};

#define MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER                                                                              \
    MYMTL_NO_DISCARD_MSG(                                                                                                \
        "This member function returns a bool indicating whether the container is empty and has no other effects. " \
        "It is not useful to call this member function and discard the return value. "                             \
        "There's no way to clear a vector as its size is fixed.")

template< typename T, std::size_t N >
    requires NumericType<T>
struct VectorN {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    static_assert(N <= static_cast<decltype(N)>((std::numeric_limits<unsigned int>::max)()), "Size can be no more than UINT_MAX");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorN_iterator<value_type, N>;
    using const_iterator = VectorN_const_iterator<value_type, N>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MYMTL_NO_DISCARD constexpr iterator begin() noexcept;
    MYMTL_NO_DISCARD constexpr iterator end() noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator begin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator end() const noexcept;

    MYMTL_NO_DISCARD constexpr reverse_iterator rbegin() noexcept;
    MYMTL_NO_DISCARD constexpr reverse_iterator rend() noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator rbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator rend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator cbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator cend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator crbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator crend() const noexcept;

    MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER constexpr bool is_empty() const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[] (size_type pos) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator[] (size_type pos) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type pos);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type pos) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    constexpr void swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr VectorN operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr VectorN& operator += (const VectorN<V, N>& rhs) noexcept(noexcept(*data() += *rhs.data()));

    template<typename V>
        requires Subtractable<T, V>
    constexpr VectorN& operator -= (const VectorN<V, N>& rhs) noexcept(noexcept(*data() -= *rhs.data()));

    template< typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator *= (U rhs) noexcept(noexcept(*data() *= rhs));

    template< typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator /= (U rhs) noexcept(noexcept(*data() /= rhs));

    value_type v[N];

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type pos);
};

//------------------------------------------------------------------------------
// Non member functions and operators
//------------------------------------------------------------------------------
// operator !=( const VectorN< T >& lhs, const VectorN< T >& rhs ) generated by compiler since C++20
template<typename T, std::size_t N>
MYMTL_NO_DISCARD constexpr bool operator == (const VectorN<T, N>& lhs, const VectorN<T, N>& rhs) noexcept;

template<typename T, std::size_t N>
    requires (N == 0) || std::swappable<T>
constexpr void swap(VectorN<T, N>&lhs, VectorN<T, N>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>()), std::size_t N>
    requires (N > 0) && NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, N> operator + (const VectorN<T, N>& lhs, const VectorN<V, N>& rhs) noexcept(noexcept(*lhs.data() + *rhs.data()));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>()), std::size_t N>
    requires (N > 0) && NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, N> operator - (const VectorN<T, N>& lhs, const VectorN<V, N>& rhs) noexcept(noexcept(*lhs.data() - *rhs.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>()), std::size_t N>
    requires (N > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, N> operator * (const VectorN<T, N>& lhs, U rhs) noexcept(noexcept(*lhs.data()* rhs));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>()), std::size_t N>
    requires (N > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, N> operator * (U lhs, const VectorN<T, N>& rhs) noexcept(noexcept(rhs* lhs));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>()), std::size_t N>
    requires (N > 0) && NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, N> operator / (const VectorN<T, N>& lhs, U rhs) noexcept(noexcept(*lhs.data() / rhs));
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// 0D vector as specialisation of VectorN<T, N>
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct VectorN<T, 0> {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorN_iterator<value_type, 0>;
    using const_iterator = VectorN_const_iterator<value_type, 0>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MYMTL_NO_DISCARD constexpr iterator begin() noexcept;
    MYMTL_NO_DISCARD constexpr iterator end() noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator begin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator end() const noexcept;

    MYMTL_NO_DISCARD constexpr reverse_iterator rbegin() noexcept;
    MYMTL_NO_DISCARD constexpr reverse_iterator rend() noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator rbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator rend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator cbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator cend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator crbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator crend() const noexcept;

    MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER constexpr bool is_empty() const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[] (size_type pos) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator[] (size_type pos) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type pos);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type pos) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    constexpr void swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr VectorN operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr VectorN& operator += (const VectorN<V, 0>& rhs) noexcept;

    template<typename V>
        requires Subtractable<T, V>
    constexpr VectorN& operator -= (const VectorN<V, 0>& rhs) noexcept;

    template< typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator *= (U rhs) noexcept;

    template< typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator /= (U rhs) noexcept;

private:
    [[noreturn]] static constexpr void range_bound_violation();
};

//------------------------------------------------------------------------------
// 0D vector non member operators declaration
//------------------------------------------------------------------------------
// operator !=( const VectorN< T >& lhs, const VectorN< T >& rhs ) generated by compiler since C++20
template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const VectorN<T, 0>& lhs, const VectorN<T, 0>& rhs) noexcept;

// Relies on generic imlementation which calls specialised member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(VectorN<T, 0>&lhs, VectorN<T, 0>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 0> operator + (const VectorN<T, 0>& lhs, const VectorN<V, 0>& rhs) noexcept;

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 0> operator - (const VectorN<T, 0>& lhs, const VectorN<V, 0>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 0> operator * (const VectorN<T, 0>& lhs, U rhs) noexcept;

// Relies on generic imlementation which calls vectorN * scalar operator above
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 0> operator * (U lhs, const VectorN<T, 0>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 0> operator / (const VectorN<T, 0>& lhs, U rhs) noexcept;
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// 1D vector as specialisation of VectorN<T, N>
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct VectorN<T, 1> {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorN_iterator<value_type, 1>;
    using const_iterator = VectorN_const_iterator<value_type, 1>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MYMTL_NO_DISCARD constexpr iterator begin() noexcept;
    MYMTL_NO_DISCARD constexpr iterator end() noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator begin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator end() const noexcept;

    MYMTL_NO_DISCARD constexpr reverse_iterator rbegin() noexcept;
    MYMTL_NO_DISCARD constexpr reverse_iterator rend() noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator rbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator rend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator cbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator cend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator crbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator crend() const noexcept;

    MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER constexpr bool is_empty() const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[] (size_type pos) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator[] (size_type pos) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type pos);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type pos) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    constexpr void swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr VectorN operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr VectorN& operator += (const VectorN<V, 1>& rhs) noexcept(noexcept(x += rhs.x));

    template<typename V>
        requires Subtractable<T, V>
    constexpr VectorN& operator -= (const VectorN<V, 1>& rhs) noexcept(noexcept(x -= rhs.x));

    template< typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator *= (U rhs) noexcept(noexcept(x *= rhs));

    template< typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator /= (U rhs) noexcept(noexcept(x /= rhs));

    value_type x;

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type pos);
};

//------------------------------------------------------------------------------
// 1D vector non member operators declaration
//------------------------------------------------------------------------------
// operator !=( const VectorN< T >& lhs, const VectorN< T >& rhs ) generated by compiler since C++20
template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const VectorN<T, 1>& lhs, const VectorN<T, 1>& rhs) noexcept;

// Relies on generic imlementation which calls specialised member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(VectorN<T, 1>&lhs, VectorN<T, 1>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 1> operator + (const VectorN<T, 1>& lhs, const VectorN<V, 1>& rhs) noexcept(noexcept(lhs.x + rhs.x));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 1> operator - (const VectorN<T, 1>& lhs, const VectorN<V, 1>& rhs) noexcept(noexcept(lhs.x - rhs.x));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 1> operator * (const VectorN<T, 1>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs));

// Relies on generic imlementation which calls vectorN * scalar operator above
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 1> operator * (U lhs, const VectorN<T, 1>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 1> operator / (const VectorN<T, 1>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs));
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// 2D vector as specialisation of VectorN<T, N>
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct VectorN<T, 2> {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorN_iterator<value_type, 2>;
    using const_iterator = VectorN_const_iterator<value_type, 2>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MYMTL_NO_DISCARD constexpr iterator begin() noexcept;
    MYMTL_NO_DISCARD constexpr iterator end() noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator begin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator end() const noexcept;

    MYMTL_NO_DISCARD constexpr reverse_iterator rbegin() noexcept;
    MYMTL_NO_DISCARD constexpr reverse_iterator rend() noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator rbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator rend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator cbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator cend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator crbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator crend() const noexcept;

    MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER constexpr bool is_empty() const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[] (size_type pos) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator[] (size_type pos) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type pos);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type pos) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    constexpr void swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr VectorN operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr VectorN& operator += (const VectorN<V, 2>& rhs) noexcept(noexcept(x += rhs.x));

    template<typename V>
        requires Subtractable<T, V>
    constexpr VectorN& operator -= (const VectorN<V, 2>& rhs) noexcept(noexcept(x -= rhs.x));

    template< typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator *= (U rhs) noexcept(noexcept(x *= rhs));

    template< typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator /= (U rhs) noexcept(noexcept(x /= rhs));

#pragma warning(disable : 4201)
    union {
        struct {
            value_type x;
            value_type y;
        };
        value_type v[2];
    };
#pragma warning(default : 4201)

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type pos);
};

//------------------------------------------------------------------------------
// 2D vector non member operators declaration
//------------------------------------------------------------------------------
// operator !=( const VectorN< T >& lhs, const VectorN< T >& rhs ) generated by compiler since C++20
template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const VectorN<T, 2>& lhs, const VectorN<T, 2>& rhs) noexcept;

// Relies on generic imlementation which calls specialised member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(VectorN<T, 2>&lhs, VectorN<T, 2>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 2> operator + (const VectorN<T, 2>& lhs, const VectorN<V, 2>& rhs) noexcept(noexcept(lhs.x + rhs.x));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 2> operator - (const VectorN<T, 2>& lhs, const VectorN<V, 2>& rhs) noexcept(noexcept(lhs.x - rhs.x));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 2> operator * (const VectorN<T, 2>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs));

// Relies on generic imlementation which calls vectorN * scalar operator above
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 2> operator * (U lhs, const VectorN<T, 2>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 2> operator / (const VectorN<T, 2>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs));
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// 3D vector as specialisation of VectorN<T, N>
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct VectorN<T, 3> {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorN_iterator<value_type, 3>;
    using const_iterator = VectorN_const_iterator<value_type, 3>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MYMTL_NO_DISCARD constexpr iterator begin() noexcept;
    MYMTL_NO_DISCARD constexpr iterator end() noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator begin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator end() const noexcept;

    MYMTL_NO_DISCARD constexpr reverse_iterator rbegin() noexcept;
    MYMTL_NO_DISCARD constexpr reverse_iterator rend() noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator rbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator rend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator cbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator cend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator crbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator crend() const noexcept;

    MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER constexpr bool is_empty() const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[] (size_type pos) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator[] (size_type pos) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type pos);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type pos) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    constexpr void swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr VectorN operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr VectorN& operator += (const VectorN<V, 3>& rhs) noexcept(noexcept(x += rhs.x));

    template<typename V>
        requires Subtractable<T, V>
    constexpr VectorN& operator -= (const VectorN<V, 3>& rhs) noexcept(noexcept(x -= rhs.x));

    template< typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator *= (U rhs) noexcept(noexcept(x *= rhs));

    template< typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator /= (U rhs) noexcept(noexcept(x /= rhs));

#pragma warning(disable : 4201)
    union {
        struct {
            value_type x;
            value_type y;
            value_type z;
        };
        value_type v[3];
    };
#pragma warning(default : 4201)

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type pos);
};

//------------------------------------------------------------------------------
// 3D vector non member operators declaration
//------------------------------------------------------------------------------
// operator !=( const VectorN< T >& lhs, const VectorN< T >& rhs ) generated by compiler since C++20
template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const VectorN<T, 3>& lhs, const VectorN<T, 3>& rhs) noexcept;

// Relies on generic imlementation which calls specialised member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(VectorN<T, 3>&lhs, VectorN<T, 3>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 3> operator + (const VectorN<T, 3>& lhs, const VectorN<V, 3>& rhs) noexcept(noexcept(lhs.x + rhs.x));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 3> operator - (const VectorN<T, 3>& lhs, const VectorN<V, 3>& rhs) noexcept(noexcept(lhs.x - rhs.x));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 3> operator * (const VectorN<T, 3>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs));

// Relies on generic imlementation which calls vectorN * scalar operator above
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 3> operator * (U lhs, const VectorN<T, 3>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 3> operator / (const VectorN<T, 3>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs));
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 4D vector as specialisation of VectorN<T, N>
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct VectorN<T, 4> {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = VectorN_iterator<value_type, 4>;
    using const_iterator = VectorN_const_iterator<value_type, 4>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MYMTL_NO_DISCARD constexpr iterator begin() noexcept;
    MYMTL_NO_DISCARD constexpr iterator end() noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator begin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator end() const noexcept;

    MYMTL_NO_DISCARD constexpr reverse_iterator rbegin() noexcept;
    MYMTL_NO_DISCARD constexpr reverse_iterator rend() noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator rbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator rend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_iterator cbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_iterator cend() const noexcept;

    MYMTL_NO_DISCARD constexpr const_reverse_iterator crbegin() const noexcept;
    MYMTL_NO_DISCARD constexpr const_reverse_iterator crend() const noexcept;

    MYMTL_NO_DISCARD_EMPTY_VECTOR_MEMBER constexpr bool is_empty() const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[] (size_type pos) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator[] (size_type pos) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type pos);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type pos) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    constexpr void swap(VectorN& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr VectorN operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr VectorN& operator += (const VectorN<V, 4>& rhs) noexcept(noexcept(x += rhs.x));

    template<typename V>
        requires Subtractable<T, V>
    constexpr VectorN& operator -= (const VectorN<V, 4>& rhs) noexcept(noexcept(x -= rhs.x));

    template< typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator *= (U rhs) noexcept(noexcept(x *= rhs));

    template< typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr VectorN& operator /= (U rhs) noexcept(noexcept(x /= rhs));

#pragma warning(disable : 4201)
    union {
        struct {
            value_type x;
            value_type y;
            value_type z;
            value_type w;
        };
        value_type v[4];
    };
#pragma warning(default : 4201)

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type pos);
};

//------------------------------------------------------------------------------
// 4D vector non member operators declaration
//------------------------------------------------------------------------------
// operator !=( const VectorN<T>& lhs, const VectorN< T >& rhs ) generated by compiler since C++20
template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const VectorN<T, 4>& lhs, const VectorN<T, 4>& rhs) noexcept;

// Relies on generic imlementation which calls specialised member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(VectorN<T, 4>&lhs, VectorN<T, 4>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 4> operator + (const VectorN<T, 4>& lhs, const VectorN<V, 4>& rhs) noexcept(noexcept(lhs.x + rhs.x));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr VectorN<R, 4> operator - (const VectorN<T, 4>& lhs, const VectorN<V, 4>& rhs) noexcept(noexcept(lhs.x - rhs.x));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 4> operator * (const VectorN<T, 4>& lhs, U rhs) noexcept(noexcept(lhs.x* rhs));

// Relies on generic imlementation which calls vectorN * scalar operator above
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R> && Multipliable<T, U> && std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr VectorN<R, 4> operator * (U lhs, const VectorN<T, 4>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr VectorN<R, 4> operator / (const VectorN<T, 4>& lhs, U rhs) noexcept(noexcept(lhs.x / rhs));
//------------------------------------------------------------------------------


template<typename R, std::size_t M, typename T, std::size_t N>
    requires std::convertible_to<T, R>
MYMTL_NO_DISCARD constexpr VectorN<R, M> to_vector(const VectorN<T, N>& src, R remaining_elements_init = {}) noexcept;


//------------------------------------------------------------------------------
// Computes dot/inner product
//------------------------------------------------------------------------------
template<typename T, typename U, std::size_t N, typename R = decltype(std::declval<T>()* std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_NO_DISCARD constexpr R vector_dot(const VectorN<T, N>& v1, const VectorN<U, N>& v2, R init = R{}) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>()* std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_NO_DISCARD constexpr R vector_dot(const VectorN<T, 1>& v1, const VectorN<U, 1>& v2, R init = R{}) noexcept(noexcept(*v1.data()** v2.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>()* std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_NO_DISCARD constexpr R vector_dot(const VectorN<T, 2>& v1, const VectorN<U, 2>& v2, R init = R{}) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>()* std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_NO_DISCARD constexpr R vector_dot(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2, R init = R{}) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>()* std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_NO_DISCARD constexpr R vector_dot(const VectorN<T, 4>& v1, const VectorN<U, 4>& v2, R init = R{}) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data()));

//------------------------------------------------------------------------------
// Computes inner product of the given vectors
//------------------------------------------------------------------------------
template< typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>
MYMTL_NO_DISCARD constexpr VectorN<R, 1> vector_cross(const VectorN<T, 1>& v1, const VectorN<U, 1>& v2) noexcept;

template< typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>()), R>
MYMTL_NO_DISCARD constexpr VectorN<R, 3> vector_cross(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2) noexcept(noexcept(*v1.data() + *v2.data()) && noexcept(*v1.data()** v2.data()));

//------------------------------------------------------------------------------
// Computes scalar triple product of vectors
//------------------------------------------------------------------------------
template<typename R, typename T, typename U, typename V>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& Multipliable<T, V>&& Addable<T, V>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>() + std::declval<V>()), R>
MYMTL_NO_DISCARD constexpr R vector_scalar_triple_ert(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2, const VectorN<V, 3>& v3) noexcept(noexcept(*v1.data() + *v2.data() + *v3.data()) && noexcept(*v1.data()** v2.data()** v3.data()));

template<typename T, typename U, typename V, typename R = decltype(std::declval<T>()* std::declval<U>()* std::declval<V>())>
    requires NumericType<R>&& Multipliable<T, U>&& Addable<T, U>&& Multipliable<T, V>&& Addable<T, V>&& std::is_convertible_v<decltype(std::declval<T>()* std::declval<U>() + std::declval<V>()), R>
MYMTL_NO_DISCARD constexpr R vector_scalar_triple(const VectorN<T, 3>& v1, const VectorN<U, 3>& v2, const VectorN<V, 3>& v3) noexcept(noexcept(*v1.data() + *v2.data() + *v3.data()) && noexcept(*v1.data()** v2.data()** v3.data()));



//------------------------------------------------------------------------------
// Common types
//------------------------------------------------------------------------------
template<typename T>
using Vector0 = VectorN<T, 0>;

template<typename T>
using Vector1 = VectorN<T, 1>;

template<typename T>
using Vector2 = VectorN<T, 2>;

template<typename T>
using Vector3 = VectorN<T, 3>;

template<typename T>
using Vector4 = VectorN<T, 4>;


MYMTL_NS_END

#include "Vector-inl.h"

#endif // !MyMTL_Vector_h