#ifndef MyMTL_defs_h
#define MyMTL_defs_h

//#include <limits>
#include <cassert>

#define MYMTL_NS_BEGIN namespace mymtl {
#define MYMTL_NS_END }

#define MYMTL_INLINE inline


#define MYMTL_NO_DISCARD [[nodiscard]]
#define MYMTL_NO_DISCARD_MSG(Message) [[nodiscard(Message)]]


#endif // MyMTL_defs_h