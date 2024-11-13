#ifndef MyMTL_Matrix_std
#define MyMTL_Matrix_std

#pragma once

#include "Matrix.h"

namespace std {
	template <class T, size_t Size>
	struct pointer_traits<mymtl::MatrixNxM_const_iterator<T, Size>> {
		using pointer = mymtl::MatrixNxM_const_iterator<T, Size>;
		using element_type = const T;
		using difference_type = ptrdiff_t;

		MYMTL_NO_DISCARD static constexpr element_type* to_address(const pointer it) noexcept {
			return it.unwrapped_ptr();
		}
	};

	template <class T, size_t Size>
	struct pointer_traits<mymtl::MatrixNxM_iterator<T, Size>> {
		using pointer = mymtl::MatrixNxM_iterator<T, Size>;
		using element_type = T;
		using difference_type = ptrdiff_t;

		MYMTL_NO_DISCARD static constexpr element_type* to_address(const pointer it) noexcept {
			return it.unwrapped_ptr();
		}
	};

#ifdef MYMTL_ENABLE_STD_SPECIALIZATION
	using mymtl::MatrixNxM;

	template< typename T, size_t N, size_t M >
		requires swappable<T>
	constexpr void swap(MatrixNxM<T, N, M>& lhs, MatrixNxM<T, N, M>& rhs) noexcept(noexcept(mymtl::swap(lhs, rhs))) {
		mymtl::swap(lhs, rhs);
	}
#endif
}

#endif // MyMTL_Matrix_std