#ifndef ApeMath_inl_h
#define ApeMath_inl_h

#pragma once

#include "ApeMath.h"
#include <cmath>

MYMTL_NS_BEGIN

//inline bool IsEqual( gfloat el1, gfloat el2, gfloat Tolerance )
//{
//	return ( fabs( el1 - el2 ) < Tolerance );
//}


//------------------------------------------------------------------------------
//
// Functions for vector
//
//------------------------------------------------------------------------------
template<typename R, typename T, std::size_t N>
    requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_INLINE constexpr decltype(std::sqrt(std::declval<R>())) vector_magnitude_ert(const VectorN<T, N>& v) noexcept(noexcept(std::sqrt(std::declval<R>()))) {
    return static_cast<R>(std::sqrt(vector_dot(v, v, R{})));
}

template<typename T, std::size_t N, typename R>
    requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_INLINE constexpr decltype(std::sqrt(std::declval<R>())) vector_magnitude(const VectorN<T, N>& v) noexcept(noexcept(std::sqrt(std::declval<R>()))) {
    return std::sqrt(vector_dot(v, v, R{}));
}

template<typename R, typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr R vector_magnitude_ert(const VectorN<T, 1>& v) noexcept(std::is_nothrow_assignable_v<T, T>) {
    return static_cast<R>(v.x);
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr T vector_magnitude(const VectorN<T, 1>& v) noexcept(std::is_nothrow_assignable_v<T, T>) {
    return v.x;
}



// Return parallel vector with the unit length
template<typename R, typename T, std::size_t N>
    requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_INLINE constexpr VectorN<decltype(std::declval<T>() / std::sqrt(std::declval<R>())), N> vector_normalize_ert(const VectorN<T, N>& v) noexcept(noexcept(std::declval<T>() / std::sqrt(std::declval<R>()))) {
    return v / vector_magnitude_ert<R>(v);
}

template<typename T, std::size_t N, typename R>
    requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_INLINE constexpr VectorN<decltype(std::declval<T>() / std::sqrt(std::declval<R>())), N> vector_normalize(const VectorN<T, N>& v) noexcept(noexcept(std::declval<T>() / std::sqrt(std::declval<R>()))) {
    return v / vector_magnitude_ert<R>(v);
}



template<typename R, typename T>
    requires NumericType<R>
MYMTL_INLINE constexpr VectorN<R, 2> vector_normal_ert(const VectorN<T, 2>& v) noexcept {
    return { static_cast<R>(-v.y), static_cast<R>(v.x) };
}

template<typename T>
MYMTL_INLINE constexpr VectorN<T, 2> vector_normal(const VectorN<T, 2>& v) noexcept {
    return { -v.y, v.x };
}

//template<typename T>
//mymtl_inline auto vector_normalize( const Vector4< T >& v, Vector4< T >& ulv ) noexcept
//{
//	const auto fNorm = VecNorm( v );
//	return ( ulv = ( abs( fNorm - static_cast< T >(1.0) ) >= kEps ) ? v / sqrt( fNorm ) : v );
//}

template<typename R, typename T, typename U, ::std::size_t N, ::std::size_t M>
    requires (N* M != 0) && NumericType<R>&& NumericType<T>&& NumericType<U>
MYMTL_INLINE constexpr VectorN<R, M> vector_transform_ert(const VectorN<T, N>& v, const MatrixNxM<U, N, M>& m) noexcept(noexcept(std::declval<T>()* std::declval<U>())) {
    VectorN<R, M>res{};

    for (std::remove_cv_t<decltype(m.rows())> k = 0; k < m.rows(); k++) {
        for (std::remove_cv_t<decltype(m.cols())> j = 0; j < m.cols(); j++) {
            res[j] += static_cast<R>(v[k]) * m(k, j);
        }
    }

    return res;
}

template<typename T, typename U, typename R, ::std::size_t N, ::std::size_t M>
    requires (N* M != 0) && NumericType<R>&& NumericType<T>&& NumericType<U>
MYMTL_INLINE constexpr VectorN<R, M> vector_transform(const VectorN<T, N>& v, const MatrixNxM<U, N, M>& m) noexcept(noexcept(std::declval<T>()* std::declval<U>())) {
    return vector_transform_ert<R>(v, m);
}

template< typename R, typename X, typename Y, typename Z, typename T>
    requires NumericType<X>&& NumericType<Y>&& NumericType<Z>&& NumericType<T>
MYMTL_INLINE constexpr Vector3<R> vector_transform_ert(X x, Y y, Z z, const Matrix3<T>& m) noexcept {
    return Vector3<R> {
        .x = static_cast<R>(x) * m(0, 0) + static_cast<R>(y) * m(1, 0) + static_cast<R>(z) * m(2, 0),
            .y = static_cast<R>(x) * m(0, 1) + static_cast<R>(y) * m(1, 1) + static_cast<R>(z) * m(2, 1),
            .z = static_cast<R>(x) * m(0, 2) + static_cast<R>(y) * m(1, 2) + static_cast<R>(z) * m(2, 2)
    };
}

template<typename X, typename Y, typename Z, typename T, typename R>
    requires NumericType<X>&& NumericType<Y>&& NumericType<Z>&& NumericType<T>
MYMTL_INLINE constexpr Vector3<R> vector_transform(X x, Y y, Z z, const Matrix3<T>& m) noexcept {
    return Vector3<R> {
        .x = x * m(0, 0) + y * m(1, 0) + z * m(2, 0),
            .y = x * m(0, 1) + y * m(1, 1) + z * m(2, 1),
            .z = x * m(0, 2) + y * m(1, 2) + z * m(2, 2)
    };
}

template<typename R, typename S, typename T>
    requires NumericType<S>&& NumericType<T>
MYMTL_INLINE constexpr Vector3<R> vector_transform_ert(const Vector3<S>& v, const Matrix3<T>& m) noexcept(noexcept(std::declval<S>()* std::declval<T>())) {
    return vector_transform_ert<R>(v.x, v.y, v.z, m);
}

template<typename S, typename T, typename R>
    requires NumericType<S>&& NumericType<T>
MYMTL_INLINE constexpr Vector3<R> vector_transform(const Vector3<S>& v, const Matrix3<T>& m) noexcept(noexcept(std::declval<S>()* std::declval<T>())) {
    return vector_transform(v.x, v.y, v.z, m);
}


//Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
//	Vec3f s[2];
//	for (int i = 2; i--; ) {
//		s[i][0] = C[i] - A[i];
//		s[i][1] = B[i] - A[i];
//		s[i][2] = A[i] - P[i];
//	}
//	Vec3f u = cross(s[0], s[1]);
//	if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
//		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
//}
template<typename T, typename U>
    requires NumericType<U>&& NumericType<T>
MYMTL_INLINE constexpr Vector3<U>& vector_barycentric(Vector3<U>& barycentric, const Vector2<T>& a, const Vector2<T>& b, const Vector2<T>& c, const Vector2<T>& p) noexcept {
    const auto cross = vector_cross(Vector3<T>{ b.x - a.x, c.x - a.x, a.x - p.x }, Vector3<T>{ b.y - a.y, c.y - a.y, a.y - p.y });

    barycentric.y = static_cast<U>(cross.x) / cross.z;
    barycentric.z = static_cast<U>(cross.y) / cross.z;
    barycentric.x = 1 - barycentric.y - barycentric.z;

    return barycentric;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr Vector3<T> vector_barycentric(const Vector2<T>& a, const Vector2<T>& b, const Vector2<T>& c, const Vector2<T>& p) noexcept {
    const auto cross = vector_cross(Vector3<T>{ b.x - a.x, c.x - a.x, a.x - p.x }, Vector3<T>{ b.y - a.y, c.y - a.y, a.y - p.y });

    Vector3<T> barycentric{ .x = 0, .y = cross.x / cross.z, .z = cross.y / cross.z };
    barycentric.x = 1 - barycentric.y - barycentric.z;

    return barycentric;
}

// Assumes counterclockwise direction
template<typename T, typename U>
    requires NumericType<U>&& NumericType<T>
MYMTL_INLINE constexpr Vector3<U>& vector_barycentric(Vector3<U>& barycentric, const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, const Vector3<T>& p) noexcept {
    //n = (b - a) cross_product (c - a)
    const auto normalABC = vector_cross(b - a, c - a);
    //na = (c - b) cross_product (p - b)
    const auto normalBCP = vector_cross(c - b, p - b);
    //nb = (a - c) cross_product (p - c)
    const auto normalCAP = vector_cross(a - c, p - c);
    //nc = (b - a) cross_product (p - a)
    const auto normalABP = vector_cross(b - a, p - a);

    const auto normalABCSqr = static_cast<U>(vector_dot(normalABC, normalABC));
    barycentric.x = vector_dot(normalABC, normalBCP) / normalABCSqr;
    barycentric.y = vector_dot(normalABC, normalCAP) / normalABCSqr;
    barycentric.z = vector_dot(normalABC, normalABP) / normalABCSqr;

    return barycentric;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr Vector3<T> vector_barycentric(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, const Vector3<T>& p) noexcept {
    //n = (b - a) cross_product (c - a)
    const auto normalABC = vector_cross(b - a, c - a);
    //na = (c - b) cross_product (p - b)
    const auto normalBCP = vector_cross(c - b, p - b);
    //nb = (a - c) cross_product (p - c)
    const auto normalCAP = vector_cross(a - c, p - c);
    //nc = (b - a) cross_product (p - a)
    const auto normalABP = vector_cross(b - a, p - a);

    const auto normalABCSqr = vector_dot(normalABC, normalABC);
    return Vector3<T> {
        .x = vector_dot(normalABC, normalBCP) / normalABCSqr, .y = vector_dot(normalABC, normalCAP) / normalABCSqr, .z = vector_dot(normalABC, normalABP) / normalABCSqr
    };
}

//inline GVector4D& VectorTransform( GVector4D &res, const GVector4D &v, const Matrix4 &m )
//{
//	gfloat ovx = v.x, ovy = v.y, ovz = v.z;
//	res.x = v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41;
//	res.y = ovx * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42;
//	res.z = ovx * m._13 + ovy * m._23 + v.z * m._33 + v.w * m._43;
//	res.w = ovx * m._14 + ovy * m._24 + ovz * m._34 + v.w * m._44;
//
//	return res;
//}
//

//
//inline GVector3D& VectorTransformCoord( GVector3D &res, const GVector3D &v, const Matrix4 &m )
//{
//	gfloat ovx = v.x, ovy = v.y;
//	res.x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
//	res.y = ovx * m._12 + v.y * m._22 + v.z * m._32 + m._42;
//	res.z = ovx * m._13 + ovy * m._23 + v.z * m._33 + m._43;
//
//	return res;
//}
//
//inline GVector3D& VectorTransformNormal( GVector3D &res, const GVector3D &v, const Matrix4 &m )
//{
//	gfloat ovx = v.x, ovy = v.y;
//	res.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
//	res.y = ovx * m._12 + v.y * m._22 + v.z * m._32;
//	res.z = ovx * m._13 + ovy * m._23 + v.z * m._33;
//
//	return res;
//}
//------------------------------------------------------------------------------
//
// Functions for p
//
//------------------------------------------------------------------------------

//inline gfloat PlaneDotCoord( const GPlane3D &p, const GVector3D &v )
//{
//	return ( gfloat )( p.a * v.x + p.b * v.y + p.c * v.z + p.d );
//}
//
//
//inline gfloat PlaneDotNormal( const GPlane3D &p, const GVector3D &v )
//{
//	return ( gfloat )( p.a * v.x + p.b * v.y + p.c * v.z );
//}
//
//
//inline GPlane3D& PlaneNormalize( GPlane3D &res, const GPlane3D &p )
//{
//	gfloat normalMag = ( gfloat )sqrt( p.a * p.a + p.b * p.b + p.c * p.c );
//	return ( res = ( fabs( normalMag - ( gfloat )1.0 ) >= kEps ) ? p / normalMag : p );
//}
//
//
//inline GPlane3D& PlaneFromPointNormal( GPlane3D &res, const GVector3D &p, const GVector3D &n )
//{
//	res.a = n.x;
//	res.b = n.y;
//	res.c = n.z;
//	res.d = -VectorDot( n, p );
//
//	return res;
//}


//------------------------------------------------------------------------------
// Functions for matrix
//------------------------------------------------------------------------------
template<typename T, ::std::size_t N, ::std::size_t M>
    requires (N* M != 0) && NumericType<T>
MYMTL_INLINE constexpr MatrixNxM<T, N, M> zero_matrix() noexcept {
    return MatrixNxM<T, N, M>{ static_cast<T>(0) };
}

template<typename T, ::std::size_t N>
    requires (N > 0) && NumericType<T>
MYMTL_INLINE constexpr MatrixN<T, N> identity_matrix() noexcept {
    auto m = zero_matrix<T, N, N>();

    for (std::remove_cv_t<decltype(m.rows())> k = 0; k < m.rows(); k++) {
        m(k, k) = static_cast<T>(1);
    }

    return m;
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixN<T, 1> identity_matrix() noexcept {
    return MatrixN<T, 1> {1};
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixN<T, 2> identity_matrix() noexcept {
    return MatrixN<T, 2> {
        1, 0,
            0, 1
    };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixN<T, 3> identity_matrix() noexcept {
    return MatrixN<T, 3> {
        1, 0, 0,
            0, 1, 0,
            0, 0, 1
    };
}

template<typename T>
    requires NumericType<T>
MYMTL_INLINE constexpr MatrixN<T, 4> identity_matrix() noexcept {
    return MatrixN<T, 4> {
        1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
}

//
//inline GMatrix3x3& MatrixIdentity( GMatrix3x3 &res )
//{
//	::memset( &res._11, 0, sizeof( GMatrix3x3 ) );
//	res._11 = res._22 = res._33 = 1;
//	
//	return res;	
//}
//
//inline bool MatrixIsIdentity( const Matrix4 &m )
//{
//	return ( abs( m._11 - 1 ) < kEps ) && ( abs( m._12 ) < kEps ) && ( abs( m._13 ) < kEps ) && ( abs( m._14 ) < kEps )
//		&& ( abs( m._21 ) < kEps ) && ( abs( m._22 - 1 ) < kEps ) && ( abs( m._23 ) < kEps ) && ( abs( m._24 ) < kEps )
//		&& ( abs( m._31 ) < kEps ) && ( abs( m._32 ) < kEps ) && ( abs( m._33 - 1 ) < kEps ) && ( abs( m._34 ) < kEps )
//		&& ( abs( m._41 ) < kEps ) && ( abs( m._42 ) < kEps ) && ( abs( m._43 ) < kEps ) && ( abs( m._44 - 1 ) < kEps );
//}
//
//inline bool MatrixIsIdentity( const GMatrix3x3 &m )
//{
//	return ( abs( m._11 - 1 ) < kEps ) && ( abs( m._12 ) < kEps ) && ( abs( m._13 ) < kEps )
//		&& ( abs( m._21 ) < kEps ) && ( abs( m._22 - 1 ) < kEps ) && ( abs( m._23 ) < kEps )
//		&& ( abs( m._31 ) < kEps ) && ( abs( m._32 ) < kEps ) && ( abs( m._33 - 1 ) < kEps );
//}
//
//inline Matrix4& MatrixTranspose( Matrix4 &res, const Matrix4 &m )
//{
//	//if ( &res != &m )
//	//{
//		res._11 = m._11; res._12 = m._21; res._13 = m._31; res._14 = m._41;
//		res._21 = m._12; res._22 = m._22; res._23 = m._32; res._24 = m._42;
//		res._31 = m._13; res._32 = m._23; res._33 = m._33; res._34 = m._43;
//		res._41 = m._14; res._42 = m._24; res._43 = m._34; res._44 = m._44;
//	//}
//	//else
//	//{
//	//	gfloat buffer[ 6 ] = { m._12, m._13, m._14, m._23, m._24, m._34 };
//	//	res._11 = m._11;		res._12 = m._21;		res._13 = m._31;		res._14 = m._41;
//	//	res._21 = buffer[ 0 ];	res._22 = m._22;		res._23 = m._32;		res._24 = m._42;
//	//	res._31 = buffer[ 1 ];	res._32 = buffer[ 3 ];	res._33 = m._33;		res._34 = m._43;
//	//	res._41 = buffer[ 2 ];	res._42 = buffer[ 4 ];	res._43 = buffer[ 5 ];	res._44 = m._44;
//	//}
//	
//	return res;
//}
//
//inline Matrix4& MatrixTranslation( Matrix4 &res, gfloat x, gfloat y, gfloat z )
//{
//	MatrixIdentity( res );
//	res._41 = x; res._42 = y; res._43 = z;
//
//	return res;
//}
//
//inline GMatrix3x3& MatrixTranslation( GMatrix3x3 &res, gfloat x, gfloat y )
//{
//	MatrixIdentity( res );
//	res._31 = x; res._32 = y;
//
//	return res;
//}
//
//inline Matrix4& MatrixRotationAxis( Matrix4 &res, const GVector3D &v, gfloat angle )
//{
//	gfloat sinTheta = sin( angle ), oneSubcosTheta = 1 - cos( angle );
//	
//	GVector3D dirCosines;
//	VectorNormalize( dirCosines, v );
//	
//	// Main diagonal
//	res._11 = ( dirCosines.x * dirCosines.x - 1 ) * oneSubcosTheta + 1;
//	res._22 = ( dirCosines.y * dirCosines.y - 1 ) * oneSubcosTheta + 1;
//	res._33 = ( dirCosines.z * dirCosines.z - 1 ) * oneSubcosTheta + 1;
//	res._44 = 1;
//
//	// Use element 14 and 41 as temporary data holder
//	res._14 = dirCosines.x * dirCosines.y * oneSubcosTheta;
//	res._41 = dirCosines.z * sinTheta;
//	res._12 = res._14 + res._41; res._21 = res._14 - res._41;
//
//	res._14 = dirCosines.x * dirCosines.z * oneSubcosTheta;
//	res._41 = dirCosines.y * sinTheta;
//	res._13 = res._14 - res._41; res._31 = res._14 + res._41;
//
//	res._14 = dirCosines.y * dirCosines.z * oneSubcosTheta;
//	res._41 = dirCosines.x * sinTheta;
//	res._23 = res._14 + res._41; res._32 = res._14 - res._41;
//
//	res._14 = res._41 = res._24 = res._42 = res._34 = res._43 = 0;
//
//	return res;
//}
//
//inline Matrix4& MatrixRotationX( Matrix4 &res, gfloat angle )
//{
//	gfloat sinA = sin( angle ), cosA = cos( angle );
//
//	res._11 = 1; res._12 = 0;		res._13 = 0;	res._14 = 0;
//	res._21 = 0; res._22 = cosA;	res._23 = sinA;	res._24 = 0;
//	res._31 = 0; res._32 = -sinA;	res._33 = cosA;	res._34 = 0;
//	res._41 = 0; res._42 = 0;		res._43 = 0;	res._44 = 1;
//
//	return res;
//}
//
//inline Matrix4& MatrixRotationY( Matrix4 &res, gfloat angle )
//{
//	gfloat sinA = sin( angle ), cosA = cos( angle );
//
//	res._11 = cosA; res._12 = 0; res._13 = -sinA;	res._14 = 0;
//	res._21 = 0;	res._22 = 1; res._23 = 0;		res._24 = 0;
//	res._31 = sinA;	res._32 = 0; res._33 = cosA;	res._34 = 0;
//	res._41 = 0;	res._42 = 0; res._43 = 0;		res._44 = 1;
//
//	return res;
//}
//
//inline Matrix4& MatrixRotationZ( Matrix4 &res, gfloat angle )
//{
//	gfloat sinA = sin( angle ), cosA = cos( angle );
//
//	res._11 = cosA;		res._12 = sinA; res._13 = 0; res._14 = 0;
//	res._21 = -sinA;	res._22 = cosA;	res._23 = 0; res._24 = 0;
//	res._31 = 0;		res._32 = 0;	res._33 = 1; res._34 = 0;
//	res._41 = 0;		res._42 = 0;	res._43 = 0; res._44 = 1;
//
//	return res;
//}
//
//// Builds a matrix that rotates around the z-axis
//inline GMatrix3x3& MatrixRotation( GMatrix3x3 &res, gfloat angle )
//{
//	gfloat sinA = sin( angle ), cosA = cos( angle );
//
//	res._11 = cosA;		res._12 = sinA; res._13 = 0;
//	res._21 = -sinA;	res._22 = cosA;	res._23 = 0;
//	res._31 = 0;		res._32 = 0;	res._33 = 1;
//	
//	return res;
//}

template<typename T, typename U>
MYMTL_INLINE constexpr auto inverse_matrix(Matrix3<U>& res, U* det, const Matrix3<T>& m) noexcept {
    res(0, 0) = m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2);
    res(0, 1) = -m(0, 1) * m(2, 2) + m(2, 1) * m(0, 2);
    res(0, 2) = m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2);

    const auto d = m(0, 0) * res(0, 0) + m(1, 0) * res(0, 1) + m(2, 0) * res(0, 2);

    // If det is not null then copy matrix determinant to det
    if (det) {
        *det = static_cast<U>(d);
    }

    // If d is almost zero, return null pointer 
    if (abs(d) < kEps) {
        return false;
    }

    res(0, 0) /= d;
    res(0, 1) /= d;
    res(0, 2) /= d;

    res(1, 0) = (-m(1, 0) * m(2, 2) + m(2, 0) * m(1, 2)) / d;
    res(1, 1) = (m(0, 0) * m(2, 2) - m(2, 0) * m(0, 2)) / d;
    res(1, 2) = (-m(0, 0) * m(1, 2) + m(1, 0) * m(0, 2)) / d;

    res(2, 0) = (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) / d;
    res(2, 1) = (-m(0, 0) * m(2, 1) + m(2, 0) * m(0, 1)) / d;
    res(2, 2) = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) / d;

    return true;
}

// Builds a matrix that scales along the x-axis, the y-axis, and the z-axis
//inline Matrix4& MatrixScaling( Matrix4 &res, gfloat sx, gfloat sy, gfloat sz )
//{
//	MatrixIdentity( res );
//	res._11 = sx;	res._22 = sy;	res._33 = sz;
//	
//	return res;
//}
//
//// Builds a matrix that scales along the x-axis, the y-axis
//inline GMatrix3x3& MatrixScaling( GMatrix3x3 &res, gfloat sx, gfloat sy )
//{
//	MatrixIdentity( res );
//	res._11 = sx;	res._22 = sy;
//
//	return res;
//}
////------------------------------------------------------------------------------
//inline GVector3D QuaternionToVector( const CQuaternion& q )
//{
//	return GVector3D( q.x, q.y, q.z );
//}
//
////------------------------------------------------------------------------------
//inline GVector3D operator * ( const CMatrix44& m, const GVector3D& v )
//{
//	return GVector3D( m._11*v.x + m._12*v.y + m._13*v.z + m._14,
//					  m._21*v.x + m._22*v.y + m._23*v.z + m._24,
//					  m._31*v.x + m._32*v.y + m._33*v.z + m._34 );
//}
//
//inline GVector3D operator * ( const GVector3D& v, const CMatrix44& m )
//{
//	return GVector3D( m._11*v.x + m._21*v.y + m._31*v.z + m._41,
//					  m._12*v.x + m._22*v.y + m._32*v.z + m._42,
//					  m._13*v.x + m._23*v.y + m._33*v.z + m._43 );
//}

////******************************************************************************
////------------------------------------------------------------------------------
//	/* Функции для работы с кватернионами */
////------------------------------------------------------------------------------
//inline CQuaternion ConjugateQuaternion( const CQuaternion& q )
//{
//	return CQuaternion( -q.x, -q.y, -q.z, q.w );
//}
////------------------------------------------------------------------------------
//inline gfloat Norm( const CQuaternion& q )
//{
//	return ( q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w );
//}
////------------------------------------------------------------------------------
//inline gfloat Magnitude( const CQuaternion& q )
//{
//    return ( gfloat )sqrt( q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w );
//}
////------------------------------------------------------------------------------
//inline CQuaternion InverseQuaternion( const CQuaternion& q )
//{
//    gfloat normInv = 1.0f / Norm( q );
//    return CQuaternion( -q.x * normInv, -q.y * normInv, -q.z * normInv, q.w * normInv );
//} 
////------------------------------------------------------------------------------
//inline CQuaternion StabilizeLength( const CQuaternion& q )
//{
//   gfloat cs = ( gfloat )( fabs( q.x ) + fabs( q.y ) + fabs( q.z ) + fabs( q.w ) );   
//   return ( cs > 0 ) ? CQuaternion( q/cs ) : CQuaternion();
//}
////------------------------------------------------------------------------------
//inline CQuaternion NormalizeQuaternion( const CQuaternion& q, gfloat Tolerance ) 
//{
//	gfloat mag = Magnitude( q );
//	if ( fabs( mag - 1 ) < Tolerance ) 
//		return q;
//	
//	return ( mag < Tolerance ) ? CQuaternion( 0, 0, 0, 0 ) : q / mag;
//}
////------------------------------------------------------------------------------
//inline CQuaternion AxisAngleToQuaternion( const GVector3D& axis, gfloat angle )
//{
//	gfloat half_angle = 0.5f * angle;
//	gfloat sin_ha = ( gfloat )sin( 0.5 * half_angle );
//			
//	return CQuaternion( axis.x * sin_ha, axis.y * sin_ha, axis.z * sin_ha, ( gfloat )cos( half_angle ) );
//}
////------------------------------------------------------------------------------
//inline CQuaternion YawPitchRollToQuaternion( gfloat Pitch, gfloat Yaw, gfloat Roll )
//{
//	// Q = Qz * Qy * Qx
//	gfloat sin_hp = ( gfloat )sin( 0.5*Pitch ), cos_hp = ( gfloat )cos( 0.5*Pitch );
//	gfloat sin_hy = ( gfloat )sin( 0.5*Yaw ),   cos_hy = ( gfloat )cos( 0.5*Yaw );
//	gfloat sin_hr = ( gfloat )sin( 0.5*Roll ),  cos_hr = ( gfloat )cos( 0.5*Roll );
//	
//	return CQuaternion( sin_hp * cos_hy * cos_hr - cos_hp * sin_hy * sin_hr,
//						cos_hp * sin_hy * cos_hr + sin_hp * cos_hy * sin_hr,
//						cos_hp * cos_hy * sin_hr - sin_hp * sin_hy * cos_hr,
//						cos_hp * cos_hy * cos_hr + sin_hp * sin_hy * sin_hr );
//}
////------------------------------------------------------------------------------
//inline gfloat ScalarProduct( const CQuaternion& q1, const CQuaternion& q2 )
//{
//    return ( q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w );
//}
////------------------------------------------------------------------------------
///*
//  q( cross(v,v') + wv' + w'v, ww' - dot(v,v') )
//*/
//inline CQuaternion VectorProduct( const CQuaternion& q1, const CQuaternion& q2 )
//{
//    return CQuaternion( q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
//                        q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
//                        q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
//                        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z );
//    // 16 multiplications    12 addidtions    0 variables
//}
////------------------------------------------------------------------------------
//inline CQuaternion VectorProduct2( const CQuaternion& q1, const CQuaternion& q2 )
//{
//    gfloat t0 = (q1.x - q1.y) * (q2.y - q2.x);
//    gfloat t1 = (q1.w + q1.z) * (q2.w + q2.z);
//    gfloat t2 = (q1.w - q1.z) * (q2.y + q2.x);
//    gfloat t3 = (q1.x + q1.y) * (q2.w - q2.z);
//    gfloat t4 = (q1.x - q1.z) * (q2.z - q2.y);
//    gfloat t5 = (q1.x + q1.z) * (q2.z + q2.y);
//    gfloat t6 = (q1.w + q1.y) * (q2.w - q2.x);
//    gfloat t7 = (q1.w - q1.y) * (q2.w + q2.x);
//
//    gfloat t8 = t5 + t6 + t7;
//    gfloat t9 = gfloat( ( t4 + t8 ) * 0.5 );
//
//    return CQuaternion ( t3+t9-t6, t2+t9-t7, t1+t9-t8, t0+t9-t5 );
//}
////------------------------------------------------------------------------------
//inline CQuaternion VectorToQuaternion( const GVector3D& v )
//{
//	return CQuaternion( v.x, v.y, v.z, 0 );
//}
//
////------------------------------------------------------------------------------
//inline void QuaternionToAxisAngle( const CQuaternion& q, GVector3D& axis, gfloat& angle )
//{
//    gfloat vl = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
//
//	if ( vl < eps )
//	{
//		axis.x = axis.y = axis.z = 0;
//		angle = 0;
//	}
//	else
//	{
//		gfloat vlInv = 1.0f / vl;
//		axis *= vlInv;
//		angle = ( q.w < 0 ) ? 2.0f * ( gfloat )atan2( -vl, -q.w ) : 2.0f * ( gfloat )atan2( vl, q.w );
//	}
//}
////------------------------------------------------------------------------------
//inline CQuaternion RotationMatrixToUnitQuaternion( const CMatrix44& mat )
//{
//	const byte nxt[3] = { 1, 2, 0 };
//
//	gfloat  s, tr = mat[0][0] + mat[1][1] + mat[2][2]; 
//	byte   i, j, k;
//	CQuaternion q;
//	
//	if ( tr > 0 )
//	{
//		s = ( gfloat )sqrt( tr + 1.0 );
//		q.w = s / 2.0f;
//		s = 0.5f / s;
//		q.x = ( mat[1][2] - mat[2][1] ) * s;
//		q.y = ( mat[2][0] - mat[0][2] ) * s;
//		q.z = ( mat[0][1] - mat[1][0] ) * s;
//	}
//	else
//	{
//		i = 0;
//		if ( mat[1][1] > mat[0][0] ) 
//			i = 1;
//		if ( mat[2][2] > mat[i][i] ) 
//			i = 2;
//		j = nxt[i];		k = nxt[j];
//
//		s = ( gfloat )sqrt( ( mat[i][i] - ( mat[j][j] + mat[k][k] ) ) + 1.0 );
//		q[i] = s / 2.0f;
//
//		if ( s >= eps ) s = 0.5f / s;
//
//		q[3] = ( mat[j][k] - mat[k][j] ) * s;
//		q[j] = ( mat[i][j] + mat[j][i] ) * s;
//		q[k] = ( mat[i][k] + mat[k][i] ) * s;
//	}
//	return q;
//}
////------------------------------------------------------------------------------
//inline CQuaternion operator * ( const CMatrix44& m, const CQuaternion& q )
//{
//	return CQuaternion( m._11*q.x + m._12*q.y + m._13*q.z + m._14*q.w,
//						m._21*q.x + m._22*q.y + m._23*q.z + m._24*q.w,
//						m._31*q.x + m._32*q.y + m._33*q.z + m._34*q.w,
//						m._41*q.x + m._42*q.y + m._43*q.z + m._44*q.w );
//}
////------------------------------------------------------------------------------
//inline CQuaternion operator * ( const CQuaternion& q, const CMatrix44& m )
//{
//	return CQuaternion( m._11*q.x + m._21*q.y + m._31*q.z + m._41*q.w,
//						m._12*q.x + m._22*q.y + m._32*q.z + m._42*q.w,
//						m._13*q.x + m._23*q.y + m._33*q.z + m._43*q.w,
//						m._14*q.x + m._24*q.y + m._34*q.z + m._44*q.w );
//}
//
////******************************************************************************
////------------------------------------------------------------------------------
///* Функции для работы с матрицами */
////------------------------------------------------------------------------------
//inline CMatrix44 TransposeMatrix( const CMatrix44& mat )
//{
//	return CMatrix44( mat._11, mat._21, mat._31, mat._41,
//					  mat._12, mat._22, mat._32, mat._42,
//					  mat._13, mat._23, mat._33, mat._43,
//					  mat._14, mat._24, mat._34, mat._44 );
//}
//
//inline CMatrix44 TranslationMatrixDX( gfloat x, gfloat y, gfloat z )
//{
//	return CMatrix44( 1, 0, 0, 0,
//					  0, 1, 0, 0,
//					  0, 0, 1, 0,
//					  x, y, z, 1 );
//}
////------------------------------------------------------------------------------
//inline CMatrix44 TranslationMatrixGL( gfloat x, gfloat y, gfloat z )
//{
//	return CMatrix44( 1, 0, 0, x,
//					  0, 1, 0, y,
//					  0, 0, 1, z,
//					  0, 0, 0, 1 );
//}
////------------------------------------------------------------------------------
//inline CMatrix44 QuaternionToRotationMatrix( const CQuaternion& q )
//{
//	gfloat s = 2.0f / Norm( q );
//	gfloat _x = q.x * s, _y = q.y * s, _z = q.z * s;
//	gfloat xx = q.x * _x, yy = q.y * _y, zz = q.z * _z;
//	gfloat xy = q.x * _y, xz = q.x * _z, yz = q.y * _z;
//	gfloat wx = q.w * _x, wy = q.w * _y, wz = q.w * _z;
//	
//	return CMatrix44( 1 - yy - zz, xy - wz, xz + wy, 0,
//					  xy + wz, 1 - xx - zz, yz - wx, 0,
//					  xz - wy, yz + wx, 1 - xx - yy, 0,
//					  0,	   0,		0,			 1 );
//}
////------------------------------------------------------------------------------
//inline CMatrix44 UnitQuaternionToRotationMatrix( const CQuaternion& q )
//{
//	gfloat _2x = q.x + q.x, _2y = q.y + q.y, _2z = q.z + q.z;
//	gfloat xx = q.x * _2x, yy = q.y * _2y, zz = q.z * _2z;
//	gfloat xy = q.x * _2y, xz = q.x * _2z, yz = q.y * _2z;
//	gfloat wx = q.w * _2x, wy = q.w * _2y, wz = q.w * _2z;
//	
//	return CMatrix44( 1 - yy - zz, xy - wz, xz + wy, 0,
//					  xy + wz, 1 - xx - zz, yz - wx, 0,
//					  xz - wy, yz + wx, 1 - xx - yy, 0,
//					  0,	   0,		0,			 1 );
//}
////------------------------------------------------------------------------------
//inline CMatrix44 VectorToDiagMatrix( const GVector3D& v )
//{
//	return CMatrix44( v.x, 0, 0, 0,
//					  0, v.y, 0, 0,
//					  0, 0, v.z, 0,
//					  0, 0,  0,  1 );
//}
////------------------------------------------------------------------------------
//inline CMatrix44 QuaternionToDiagMatrix( const CQuaternion& q )
//{
//	return CMatrix44( q.x, 0, 0, 0,
//					  0, q.y, 0, 0,
//					  0, 0, q.z, 0,
//					  0, 0, 0, q.w ); 
//}
//
////------------------------------------------------------------------------------
///* Linear Interpolation */
////------------------------------------------------------------------------------
//inline void QLinearIntrp::SetQuaternions( const CQuaternion& _q1, const CQuaternion& _q2, bool UnitLength )
//{
//	q1_ = ( UnitLength ) ? _q1 : NormalizeQuaternion( _q1, eps );
//    q2_ = ( UnitLength ) ? _q2 : NormalizeQuaternion( _q2, eps );
//    
//	gfloat inner = ScalarProduct( q1_, q2_ );
//    if( inner < 0 )
//	{
//		q2_ = -q2_;
//	}
//    q2_ -= q1_;
//}
//
//inline CQuaternion QLinearIntrp::Interpolate( gfloat t )
//{
//	return q1_ + q2_*t;
//}
//
////------------------------------------------------------------------------------
///* Spherical Linear Interpolation */
////------------------------------------------------------------------------------
//inline void QSLinearIntrp::SetQuaternions( const CQuaternion& _q1, const CQuaternion& _q2, bool UnitLength )
//{
//	q1_ = ( UnitLength ) ? _q1 : NormalizeQuaternion( _q1, eps );
//	q2_ = ( UnitLength ) ? _q2 : NormalizeQuaternion( _q2, eps );
//	
//	gfloat cos_omega = ScalarProduct( q1_, q2_ );
//	if( cos_omega < 0 )
//	{
//		cos_omega = -cos_omega;
//		q2_ = -q2_;
//	}
//	if ( cos_omega > 1 ) cos_omega = 1;
//	omega_ = ( gfloat )acos( cos_omega );
//
//	gfloat inv_sin_omega = ( gfloat )( 1.0 / sin( omega_ ) );
//	q1_ *= inv_sin_omega;
//	q2_ *= inv_sin_omega;
//}
//
//inline CQuaternion QSLinearIntrp::Interpolate( gfloat t )
//{
//	return q1_ * ( gfloat )sin( (1.0 - t) * omega_ ) + q2_ * ( gfloat )sin( t*omega_ );
//}

MYMTL_NS_END

#endif // ApeMath_inl_h