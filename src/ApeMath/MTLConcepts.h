#ifndef MyMTL_Concepts_h
#define MyMTL_Concepts_h

#include "MTLValsCore.h"

#include <concepts>

MYMTL_NS_BEGIN

// Meets the DefaultConstructible, CopyConstructible, CopyAssignable and Destructible requirements
// regular = copyable<T> && default_initializable<T> && equality_comparable<T>
template<typename T>
concept NumericType = std::regular<T> && std::is_nothrow_destructible_v<T>;

template< typename T, typename U = T >
concept Addable = requires (T t, U u) { t + u; };
template< typename T, typename U = T >
concept Subtractable = requires (T t, U u) { t - u; };
template< typename T, typename U = T >
concept Multipliable = requires (T t, U u) { t * u; };
template< typename T, typename U = T >
concept Dividable = requires (T t, U u) { t / u; };

MYMTL_NS_END

#endif //MyMTL_Concepts_h