#ifndef MyMTL_Matrix_h
#define MyMTL_Matrix_h

#include "MTLValsCore.h"
#include "MTLConcepts.h"

#include <iterator>
#include <limits>

//https://github.com/KTXSoftware/kfx/blob/master/Libraries/DirectX/Include/xnamath.h

MYMTL_NS_BEGIN

//------------------------------------------------------------------------------
// Matrix const_iterator
//------------------------------------------------------------------------------
template <class T, std::size_t Size>
class MatrixNxM_const_iterator {
public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

#if _ITERATOR_DEBUG_LEVEL == 0
    constexpr MatrixNxM_const_iterator() noexcept;

    constexpr explicit MatrixNxM_const_iterator(pointer p, size_t offset = 0) noexcept;

    MYMTL_NO_DISCARD constexpr reference operator*() const noexcept;

    MYMTL_NO_DISCARD constexpr pointer operator->() const noexcept;

    constexpr MatrixNxM_const_iterator& operator++() noexcept;
    constexpr MatrixNxM_const_iterator operator++(int) noexcept;

    constexpr MatrixNxM_const_iterator& operator--() noexcept;
    constexpr MatrixNxM_const_iterator operator--(int) noexcept;

    constexpr MatrixNxM_const_iterator& operator+=(const difference_type offset) noexcept;
    constexpr MatrixNxM_const_iterator& operator-=(const difference_type offset) noexcept;

    MYMTL_NO_DISCARD constexpr difference_type operator-(const MatrixNxM_const_iterator& rhs) const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[](const difference_type offset) const noexcept;

    MYMTL_NO_DISCARD constexpr bool operator==(const MatrixNxM_const_iterator& rhs) const noexcept;

    // Does not see implementation in inl file, at least IntelliSense undelines it as missing implementation
    MYMTL_NO_DISCARD MYMTL_INLINE constexpr std::strong_ordering operator<=>(const MatrixNxM_const_iterator& rhs) const noexcept {
        return ptr <=> rhs.ptr;
    }

    MYMTL_NO_DISCARD constexpr pointer unwrapped_ptr() const noexcept;

    // Unused
    //constexpr void _Seek_to(pointer it) noexcept;

private:
    pointer ptr; // beginning of array
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 / _ITERATOR_DEBUG_LEVEL != 0 vvv

public:
    constexpr MatrixNxM_const_iterator() noexcept;

    constexpr explicit MatrixNxM_const_iterator(pointer p, size_t offset = 0) noexcept;

    MYMTL_NO_DISCARD constexpr reference operator*() const noexcept;

    MYMTL_NO_DISCARD constexpr pointer operator->() const noexcept;

    constexpr MatrixNxM_const_iterator& operator++() noexcept;

    constexpr MatrixNxM_const_iterator operator++(int) noexcept;

    constexpr MatrixNxM_const_iterator& operator--() noexcept;

    constexpr MatrixNxM_const_iterator operator--(int) noexcept;

    constexpr MatrixNxM_const_iterator& operator+=(const difference_type offset) noexcept;

    constexpr MatrixNxM_const_iterator& operator-=(const difference_type offset) noexcept;

    MYMTL_NO_DISCARD constexpr difference_type operator-(const MatrixNxM_const_iterator& rhs) const noexcept;

    MYMTL_NO_DISCARD constexpr reference operator[](const difference_type offset) const noexcept;

    MYMTL_NO_DISCARD constexpr bool operator==(const MatrixNxM_const_iterator& rhs) const noexcept;

    MYMTL_NO_DISCARD constexpr std::strong_ordering operator<=>(const MatrixNxM_const_iterator& rhs) const noexcept {
        dbg_verify_ptr(rhs);
        return index <=> rhs.index;
    }

    MYMTL_NO_DISCARD constexpr pointer unwrapped_ptr() const noexcept;
    constexpr void dbg_verify_with(const MatrixNxM_const_iterator& last) const noexcept;

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

    MYMTL_INLINE constexpr void dbg_verify_ptr(const MatrixNxM_const_iterator& rhs) const noexcept { // test for compatible iterator pair
        _STL_VERIFY(ptr == rhs.ptr, "array iterators incompatible");
    }

private:
    pointer ptr; // beginning of array
    size_type index; // offset into array
#endif // ^^^ _ITERATOR_DEBUG_LEVEL != 0 ^^^

public:
    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr MatrixNxM_const_iterator operator+(MatrixNxM_const_iterator next, const difference_type offset) noexcept {
        MatrixNxM_const_iterator tmp = next;
        tmp += offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr MatrixNxM_const_iterator operator-(MatrixNxM_const_iterator next, const difference_type offset) noexcept {
        MatrixNxM_const_iterator tmp = next;
        tmp -= offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr MatrixNxM_const_iterator operator+(const difference_type offset, MatrixNxM_const_iterator next) noexcept {
        next += offset;
        return next;
    }
}; //MatrixNxM_const_iterator

//------------------------------------------------------------------------------
// Matrix iterator
//------------------------------------------------------------------------------
template <class T, std::size_t Size>
class MatrixNxM_iterator : public MatrixNxM_const_iterator<T, Size> {
public:
    using my_base = MatrixNxM_const_iterator<T, Size>;

    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    constexpr MatrixNxM_iterator() noexcept;

    constexpr explicit MatrixNxM_iterator(pointer p, size_t offset = 0) noexcept;

    MYMTL_NO_DISCARD constexpr reference operator*() const noexcept;

    MYMTL_NO_DISCARD constexpr pointer operator->() const noexcept;

    constexpr MatrixNxM_iterator& operator++() noexcept;
    constexpr MatrixNxM_iterator operator++(int) noexcept;

    constexpr MatrixNxM_iterator& operator--() noexcept;
    constexpr MatrixNxM_iterator operator--(int) noexcept;

    constexpr MatrixNxM_iterator& operator+=(const difference_type offset) noexcept;
    constexpr MatrixNxM_iterator& operator-=(const difference_type offset) noexcept;

    using my_base::operator-;

    MYMTL_NO_DISCARD constexpr reference operator[](const difference_type offset) const noexcept;

    //using my_base::operator<=>;

    MYMTL_NO_DISCARD constexpr pointer unwrapped_ptr() const noexcept;

    // Unused
    //constexpr void _Seek_to(pointer it) noexcept;

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr MatrixNxM_iterator operator+(MatrixNxM_iterator next, const difference_type offset) noexcept {
        MatrixNxM_iterator tmp = next;
        tmp += offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr MatrixNxM_iterator operator-(MatrixNxM_iterator next, const difference_type offset) noexcept {
        MatrixNxM_iterator tmp = next;
        tmp -= offset;
        return tmp;
    }

    friend MYMTL_NO_DISCARD MYMTL_INLINE constexpr MatrixNxM_iterator operator+(const difference_type offset, MatrixNxM_iterator next) noexcept {
        next += offset;
        return next;
    }
};


#define MYMTL_NO_DISCARD_EMPTY_MATRIX_MEMBER                                                                              \
    MYMTL_NO_DISCARD_MSG(                                                                                                \
        "This member function returns a bool indicating whether the container is empty and has no other effects. " \
        "It is not useful to call this member function and discard the return value. "                             \
        "There's no way to clear a matrix as its size is fixed.")


//------------------------------------------------------------------------------
// Matrix of fixed size
//------------------------------------------------------------------------------
template<typename T, std::size_t N, std::size_t M>
    requires NumericType<T>
struct MatrixNxM {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    static_assert(N* M <= static_cast<decltype(N)>((std::numeric_limits<unsigned int>::max)()), "Size can be no more than UINT_MAX");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = MatrixNxM_iterator<value_type, N* M>;
    using const_iterator = MatrixNxM_const_iterator<value_type, N* M>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //constexpr MatrixNxM() = default;
    //constexpr ~MatrixNxM() = default;

    //constexpr MatrixNxM(const MatrixNxM& src) = default;
    //constexpr MatrixNxM& operator = (const MatrixNxM& rhs) = default;

    //constexpr MatrixNxM(MatrixNxM&& src) = default;
    //constexpr MatrixNxM& operator = (MatrixNxM&& other) = default;

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


    MYMTL_NO_DISCARD static constexpr size_type rows() noexcept;
    MYMTL_NO_DISCARD static constexpr size_type cols() noexcept;
    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    MYMTL_NO_DISCARD_EMPTY_MATRIX_MEMBER constexpr bool is_empty() const noexcept;


    // access grants replace with operator[](size_type row, size_type col) when compiler supports it
    MYMTL_NO_DISCARD constexpr reference operator () (size_type row, size_type col) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator () (size_type row, size_type col) const noexcept;

    MYMTL_NO_DISCARD constexpr reference at(size_type row, size_type col);
    MYMTL_NO_DISCARD constexpr const_reference at(size_type row, size_type col) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    constexpr void swap(MatrixNxM& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr MatrixNxM operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr MatrixNxM& operator += (const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*data() += *rhs.data()));

    template<typename V>
        requires Subtractable<T, V>
    constexpr MatrixNxM& operator -= (const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*data() -= *rhs.data()));

    template<typename V> // only square matrices support multiplication with assignment
        requires (N == M) && Addable<T, V>&& Multipliable<T, V>
    constexpr MatrixNxM& operator *= (const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*data()** rhs.data()) && noexcept(*data() + *rhs.data()));

    template<typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr MatrixNxM& operator *= (U rhs) noexcept(noexcept(*data() *= rhs));

    template<typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr MatrixNxM& operator /= (U rhs) noexcept(noexcept(*data() /= rhs));

    value_type m[N * M];

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type row, size_type col);
};

//------------------------------------------------------------------------------
// Non member functions and operators
//------------------------------------------------------------------------------

//template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>()), std::size_t N, std::size_t M, std::size_t P>
//	requires (N > 0) && (M > 0) && (P > 0) && Addable<T, V>&& Multipliable<T, V>
//constexpr auto& matrix_multiply_r(MatrixNxM<R, N, P>& __restrict r, const MatrixNxM<T, N, M>& m1, const MatrixNxM<V, M, P>& m2) noexcept(noexcept(*m1.data()** m2.data()) && noexcept(*m1.data() + *m2.data()));

template<typename R /*= decltype(std::declval<T>() + std::declval<V>())*/, typename T, typename V, std::size_t N, std::size_t M, std::size_t P>
    requires (N > 0) && (M > 0) && (P > 0) && Addable<T, V>&& Multipliable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, P> matrix_multiply_ert(const MatrixNxM<T, N, M>& m1, const MatrixNxM<V, M, P>& m2) noexcept(noexcept(*m1.data()** m2.data()) && noexcept(*m1.data() + *m2.data()));

template<typename T, std::size_t N, std::size_t M>
    requires std::swappable<T>
constexpr void swap(MatrixNxM<T, N, M>& lhs, MatrixNxM<T, N, M>& rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T, std::size_t N, std::size_t M>
MYMTL_NO_DISCARD constexpr bool operator == (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<T, N, M>& rhs) noexcept;

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>()), std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, M> operator + (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*lhs.data() + *rhs.data()));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>()), std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, M> operator - (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<V, N, M>& rhs) noexcept(noexcept(*lhs.data() - *rhs.data()));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>()), std::size_t N, std::size_t M, std::size_t P>
    requires (N > 0) && (M > 0) && (P > 0) && NumericType<R>&& Addable<T, V>&& Multipliable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, P> operator * (const MatrixNxM<T, N, M>& lhs, const MatrixNxM<V, M, P>& rhs) noexcept(noexcept(*lhs.data()** rhs.data()) && noexcept(*lhs.data() + *rhs.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>()), std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, M> operator * (const MatrixNxM<T, N, M>& lhs, U rhs) noexcept(noexcept(*lhs.data()* rhs));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>()), std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, M> operator * (U lhs, const MatrixNxM<T, N, M>& rhs) noexcept(noexcept(rhs* lhs));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>()), std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0) && NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, N, M> operator / (const MatrixNxM<T, N, M>& lhs, U rhs) noexcept(noexcept(*lhs.data() / rhs));


template<typename T, std::size_t N, std::size_t M>
    requires (N > 0) && (M > 0)
MYMTL_NO_DISCARD constexpr MatrixNxM<T, M, N> matrix_transpose(const MatrixNxM<T, N, M>& m) noexcept(noexcept(std::is_nothrow_assignable_v<T, T>));

// Performs in-place matrix transpose
template<typename T, std::size_t N>
    requires (N > 0)
constexpr MatrixNxM<T, N, N>& matrix_transpose(MatrixNxM<T, N, N>& m) noexcept(noexcept(std::swappable<T>));


//------------------------------------------------------------------------------
// MatrixNxM<T, 0> specialisation
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct MatrixNxM<T, 0, 0> {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = MatrixNxM_iterator<value_type, 0>;
    using const_iterator = MatrixNxM_const_iterator<value_type, 0>;

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


    MYMTL_NO_DISCARD static constexpr size_type rows() noexcept;
    MYMTL_NO_DISCARD static constexpr size_type cols() noexcept;
    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    MYMTL_NO_DISCARD_EMPTY_MATRIX_MEMBER constexpr bool is_empty() const noexcept;


    // subcription ()
    MYMTL_NO_DISCARD constexpr reference operator () (size_type row, size_type col) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator () (size_type row, size_type col) const noexcept;

    [[noreturn]] constexpr reference at(size_type row, size_type col);
    [[noreturn]] constexpr const_reference at(size_type row, size_type col) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    constexpr void swap(MatrixNxM& other) noexcept;

    // unary operators
    MYMTL_NO_DISCARD constexpr MatrixNxM operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr MatrixNxM& operator += (const MatrixNxM<V, 0, 0>& rhs) noexcept;

    template<typename V>
        requires Subtractable<T, V>
    constexpr MatrixNxM& operator -= (const MatrixNxM<V, 0, 0>& rhs) noexcept;

    template<typename V>
        requires Addable<T, V>&& Multipliable<T, V>
    constexpr MatrixNxM& operator *= (const MatrixNxM<V, 0, 0>& rhs) noexcept;

    template<typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr MatrixNxM& operator *= (U val) noexcept;

    template<typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr MatrixNxM& operator /= (U val) noexcept;

private:
    [[noreturn]] static constexpr void range_bound_violation();
};


//------------------------------------------------------------------------------
// Non member functions and operators
//------------------------------------------------------------------------------
template<typename R, typename T, typename V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> matrix_multiply_ert(const MatrixNxM<T, 0, 0>& m1, const MatrixNxM<V, 0, 0>& m2) noexcept;

// Relies on generic imlementation which calls swap() member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(MatrixNxM<T, 0, 0>&lhs, MatrixNxM<T, 0, 0>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<T, 0, 0>& rhs) noexcept;

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> operator + (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<V, 0, 0>& rhs) noexcept;

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> operator - (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<V, 0, 0>& rhs) noexcept;

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>&& Multipliable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> operator * (const MatrixNxM<T, 0, 0>& lhs, const MatrixNxM<V, 0, 0>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> operator * (const MatrixNxM<T, 0, 0>& lhs, U rhs) noexcept;

// Relies on generic imlementation which calls operator * (const MatrixNxM<T, 0, 0>& lhs, U rhs) member function
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> operator * (U lhs, const MatrixNxM<T, 0, 0>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 0, 0> operator / (const MatrixNxM<T, 0, 0>& lhs, U rhs) noexcept;


template<typename T>
MYMTL_NO_DISCARD constexpr MatrixNxM<T, 0, 0> matrix_transpose(MatrixNxM<T, 0, 0> m) noexcept;


//------------------------------------------------------------------------------
// MatrixNxM<T, 1> specialisation
//------------------------------------------------------------------------------
template<typename T>
    requires NumericType<T>
struct MatrixNxM< T, 1, 1 > {
    static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
        "because of [container.requirements].");

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = MatrixNxM_iterator<value_type, 1>;
    using const_iterator = MatrixNxM_const_iterator<value_type, 1>;

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


    MYMTL_NO_DISCARD static constexpr size_type rows() noexcept;
    MYMTL_NO_DISCARD static constexpr size_type cols() noexcept;
    MYMTL_NO_DISCARD static constexpr size_type size() noexcept;

    MYMTL_NO_DISCARD_EMPTY_MATRIX_MEMBER constexpr bool is_empty() const noexcept;


    // subcription ()
    MYMTL_NO_DISCARD constexpr reference operator () (size_type row, size_type col) noexcept;
    MYMTL_NO_DISCARD constexpr const_reference operator () (size_type row, size_type col) const noexcept;

    [[noreturn]] constexpr reference at(size_type row, size_type col);
    [[noreturn]] constexpr const_reference at(size_type row, size_type col) const;

    MYMTL_NO_DISCARD constexpr pointer data() noexcept;
    MYMTL_NO_DISCARD constexpr const_pointer data() const noexcept;

    constexpr void swap(MatrixNxM& other) noexcept(std::is_nothrow_swappable_v<T>);

    // unary operators
    MYMTL_NO_DISCARD constexpr MatrixNxM operator - () const noexcept;

    // arithmetic operators
    template<typename V>
        requires Addable<T, V>
    constexpr MatrixNxM& operator += (const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*data() += *rhs.data()));

    template<typename V>
        requires Subtractable<T, V>
    constexpr MatrixNxM& operator -= (const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*data() -= *rhs.data()));

    template<typename V>
        requires Addable<T, V>&& Multipliable<T, V>
    constexpr MatrixNxM& operator *= (const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*data() *= *rhs.data()));

    template<typename U >
        requires Multipliable<T, U>&& std::convertible_to<U, T>
    constexpr MatrixNxM& operator *= (U rhs) noexcept(noexcept(*data() *= rhs));

    template<typename U >
        requires Dividable<T, U>&& std::convertible_to<U, T>
    constexpr MatrixNxM& operator /= (U rhs) noexcept(noexcept(*data() /= rhs));

    MYMTL_NO_DISCARD constexpr auto operator <=> (const MatrixNxM& rhs) const noexcept(noexcept(m11 <=> rhs.m11)) = default;

    value_type m11;

private:
    [[noreturn]] static constexpr void range_bound_violation(size_type row, size_type col);
};


//------------------------------------------------------------------------------
// Non member functions and operators
//------------------------------------------------------------------------------
template<typename R, typename T, typename V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> matrix_multiply_ert(const MatrixNxM<T, 1, 1>& m1, const MatrixNxM<V, 1, 1>& m2) noexcept(noexcept(*m1.data()** m2.data()));

// Relies on generic imlementation which calls swap() member function
//template<typename T>
//	requires std::swappable<T>
//constexpr void swap(MatrixNxM<T, 1, 1>&lhs, MatrixNxM<T, 1, 1>&rhs) noexcept(noexcept(lhs.swap(rhs)));

template<typename T>
MYMTL_NO_DISCARD constexpr bool operator == (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<T, 1, 1>& rhs) noexcept;

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> operator + (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*lhs.data() + *rhs.data()));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Subtractable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> operator - (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*lhs.data() - *rhs.data()));

template<typename T, typename V, typename R = decltype(std::declval<T>() + std::declval<V>())>
    requires NumericType<R>&& Addable<T, V>&& Multipliable<T, V>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> operator * (const MatrixNxM<T, 1, 1>& lhs, const MatrixNxM<V, 1, 1>& rhs) noexcept(noexcept(*lhs.data()** rhs.data()));

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T> && (!std::same_as<U, MatrixNxM<T, 1, 1>>)
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> operator * (const MatrixNxM<T, 1, 1>& lhs, U rhs) noexcept(noexcept(*lhs.data()* rhs));

// Relies on generic imlementation which calls operator * (const MatrixNxM<T, 1, 1>& lhs, U rhs) member function
//template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
//	requires NumericType<R>&& Multipliable<T, U>&& std::convertible_to<U, T>
//MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> operator * (U lhs, const MatrixNxM<T, 1, 1>& rhs) noexcept;

template<typename T, typename U, typename R = decltype(std::declval<T>() + std::declval<U>())>
    requires NumericType<R>&& Dividable<T, U>&& std::convertible_to<U, T>
MYMTL_NO_DISCARD constexpr MatrixNxM<R, 1, 1> operator / (const MatrixNxM<T, 1, 1>& lhs, U rhs) noexcept(noexcept(*lhs.data() / rhs));


template<typename T>
MYMTL_NO_DISCARD constexpr MatrixNxM<T, 1, 1> matrix_transpose(const MatrixNxM<T, 1, 1>& m) noexcept(noexcept(std::is_nothrow_assignable_v<T, T>));

// Performs in-place matrix transpose, but for 1x1 this is no op, returns m
template<typename T>
constexpr MatrixNxM<T, 1, 1>& matrix_transpose(MatrixNxM<T, 1, 1>& m) noexcept;

//------------------------------------------------------------------------------
// Common types
//------------------------------------------------------------------------------
template<typename T, std::size_t N>
    requires NumericType<T>
using MatrixN = MatrixNxM<T, N, N>;

template<typename T>
    requires NumericType<T>
using Matrix0 = MatrixN<T, 0>;

template<typename T>
    requires NumericType<T>
using Matrix1 = MatrixN<T, 1>;

template<typename T>
    requires NumericType<T>
using Matrix2 = MatrixN<T, 2>;

template<typename T>
    requires NumericType<T>
using Matrix3 = MatrixN<T, 3>;

template<typename T>
    requires NumericType<T>
using Matrix4 = MatrixN<T, 4>;


MYMTL_NS_END

#include "Matrix-inl.h"

#endif // MyMTL_Matrix_h
