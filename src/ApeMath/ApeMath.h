#ifndef ApeMath_h
#define ApeMath_h

#pragma once

#include "MTLValsCore.h"

#include "Vector.h"
#include "Matrix.h"
#include "MTLConcepts.h"

#include <cmath>
#include <numbers>

MYMTL_NS_BEGIN

using gfloat = long double;

constexpr auto kFuzzFactor = 100;
constexpr auto kEps = kFuzzFactor * std::numeric_limits< gfloat >::epsilon();

// Pi constants
constexpr auto kPI = std::numbers::pi_v<gfloat>;
constexpr auto kInvPI = std::numbers::inv_pi_v<gfloat>;
constexpr auto kHalfPI = kPI / 2;

// Exp constants
constexpr auto kExp = std::numbers::e_v<gfloat>;


using Vector2i = Vector2<int>;
using Vector2l = Vector2<long>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

using Vector3i = Vector3<int>;
using Vector3l = Vector3<long>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

using Vector4i = Vector4<int>;
using Vector4l = Vector4<long>;
using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;

using Matrix3i = Matrix3<int>;
using Matrix3l = Matrix3<long>;
using Matrix3f = Matrix3<float>;
using Matrix3d = Matrix3<double>;


//------------------------------------------------------------------------------
// Vector functions
//------------------------------------------------------------------------------

// Calculates squared length of the vector


// Calculates length of the vector
template<typename R, typename T, std::size_t N>
	requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_NO_DISCARD constexpr decltype(std::sqrt(std::declval<R>())) vector_magnitude_ert(const VectorN<T, N>& v) noexcept(noexcept(std::sqrt(std::declval<R>())));

template<typename T, std::size_t N, typename R = decltype(std::declval<T>()* std::declval<T>())>
	requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_NO_DISCARD constexpr decltype(std::sqrt(std::declval<R>())) vector_magnitude(const VectorN<T, N>& v) noexcept(noexcept(std::sqrt(std::declval<R>())));

template<typename R, typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr R vector_magnitude_ert(const VectorN<T, 1>& v) noexcept(std::is_nothrow_assignable_v<T, T>);

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr T vector_magnitude(const VectorN<T, 1>& v) noexcept(std::is_nothrow_assignable_v<T, T>);

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Return parallel vector with the unit length
//------------------------------------------------------------------------------
template<typename R, typename T, std::size_t N>
	requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_NO_DISCARD constexpr VectorN<decltype(std::declval<T>() / std::sqrt(std::declval<R>())), N> vector_normalize_ert(const VectorN<T, N>& v) noexcept(noexcept(std::declval<T>() / std::sqrt(std::declval<R>())));

template<typename T, std::size_t N, typename R = decltype(std::declval<T>()* std::declval<T>())>
	requires (N > 0) && NumericType<R>&& Multipliable<T, T>
MYMTL_NO_DISCARD constexpr VectorN<decltype(std::declval<T>() / std::sqrt(std::declval<R>())), N> vector_normalize(const VectorN<T, N>& v) noexcept(noexcept(std::declval<T>() / std::sqrt(std::declval<R>())));

//template<typename T>
//auto vector_normalize( const Vector4< T >& v, Vector4< T >& ulv ) noexcept;
//------------------------------------------------------------------------------
template<typename R, typename T>
	requires NumericType<R>
MYMTL_NO_DISCARD constexpr VectorN<R, 2> vector_normal_ert(const VectorN<T, 2>& v) noexcept;

template<typename T>
MYMTL_NO_DISCARD constexpr VectorN<T, 2> vector_normal(const VectorN<T, 2>& v) noexcept;


// Transforms a vector by a given matrix
template< typename R, typename T, typename U, ::std::size_t N, ::std::size_t M>
	requires (N* M != 0) && NumericType<R>&& NumericType<T>&& NumericType<U>
MYMTL_NO_DISCARD constexpr VectorN<R, M> vector_transform_ert(const VectorN<T, N>& v, const MatrixNxM<U, N, M>& m) noexcept(noexcept(std::declval<T>()* std::declval<U>()));

template< typename T, typename U, typename R = decltype(std::declval<T>()* std::declval<U>()), ::std::size_t N, ::std::size_t M>
	requires (N* M != 0) && NumericType<R>&& NumericType<T>&& NumericType<U>
MYMTL_NO_DISCARD constexpr VectorN<R, M> vector_transform(const VectorN<T, N>& v, const MatrixNxM<U, N, M>& m) noexcept(noexcept(std::declval<T>()* std::declval<U>()));

template< typename R, typename X, typename Y, typename Z, typename T>
	requires NumericType<X>&& NumericType<Y>&& NumericType<Z>&& NumericType<T>
MYMTL_NO_DISCARD constexpr Vector3<R> vector_transform_ert(X x, Y y, Z z, const Matrix3<T>& m) noexcept;

template< typename X, typename Y, typename Z, typename T, typename R = decltype(std::declval<T>()* std::declval<X>() + std::declval<T>() * std::declval<Y>() + std::declval<T>() * std::declval<Z>())>
	requires NumericType<X>&& NumericType<Y>&& NumericType<Z>&& NumericType<T>
MYMTL_NO_DISCARD constexpr Vector3<R> vector_transform(X x, Y y, Z z, const Matrix3<T>& m) noexcept;

template<typename R, typename S, typename T>
	requires NumericType<S>&& NumericType<T>
MYMTL_NO_DISCARD constexpr Vector3<R> vector_transform_ert(const Vector3<S>& v, const Matrix3<T>& m) noexcept(noexcept(std::declval<S>()* std::declval<T>()));

template<typename S, typename T, typename R = decltype(std::declval<T>()* std::declval<S>())>
	requires NumericType<S>&& NumericType<T>
MYMTL_NO_DISCARD constexpr Vector3<R> vector_transform(const Vector3<S>& v, const Matrix3<T>& m) noexcept(noexcept(std::declval<S>()* std::declval<T>()));



//template< typename T, typename U >
//Vector4<T>& VectorTransform( Vector4<T>& res, const Vector4<U>& v, const Matrix4<T>& m );

template<typename T, typename U>
	requires NumericType<U>&& NumericType<T>
constexpr Vector3<U>& vector_barycentric(Vector3<U>& bc, const Vector2<T>& a, const Vector2<T>& b, const Vector2<T>& c, const Vector2<T>& p) noexcept;

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr Vector3<T> vector_barycentric(const Vector2<T>& a, const Vector2<T>& b, const Vector2<T>& c, const Vector2<T>& p) noexcept;

template<typename T, typename U>
	requires NumericType<U>&& NumericType<T>
constexpr Vector3<U>& vector_barycentric(Vector3<U>& barycentric, const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, const Vector3<T>& p) noexcept;

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr Vector3<T> vector_barycentric(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, const Vector3<T>& p) noexcept;

// This function transforms the vector, pV (x, y, z, 1), by the matrix, pM,
// projecting the result back into w=1
//	GVector3D& VectorTransformCoord( GVector3D &res, const GVector3D &v, const Matrix4 &m );

	// Transforms the 3-D vector normal by the given matrix
//	GVector3D& VectorTransformNormal( GVector3D &res, const GVector3D &v, const Matrix4 &m );

	// This function transforms the vector, v(x, y, 1), by the matrix, m,
	// projecting the result back into z=1
	//GVector2D& VectorTransformCoord( GVector2D &res, const GVector2D &v, const GMatrix3x3 &m );

	// Transforms the 2-D vector normal by the given matrix
	//GVector2D& VectorTransformNormal( GVector2D &res, const GVector2D &v, const GMatrix3x3 &m );

	//GVector3D QuaternionToVector( const CQuaternion& q );


//------------------------------------------------------------------------------
//
// Plane functions
//
//------------------------------------------------------------------------------

	// Returns dot product of plane's equation and 4D vector
	//gfloat PlaneDot( const GPlane3D &plane, const GVector4D vector );

	// Computes dot product between plane and 3D vector i.e.
	// given a plane (a, b, c, d) and a 3-D vector (x, y, z)
	// the return value of this function is a*x + b*y + c*z + d*1
	//gfloat PlaneDotCoord( const GPlane3D &p, const GVector3D &v );

	// Computes dot product between plane and 3D vector
	// given a plane (a, b, c, d) and a 3-D vector (x, y, z)
	// the return value of this function is a*x + b*y + c*z + d*0
	//gfloat PlaneDotNormal( const GPlane3D &p, const GVector3D &v );

	// Normalizes the plane coefficients so that the plane normal has unit length
	// This function normalizes a plane so that |a,b,c| == 1
	//GPlane3D& PlaneNormalize( GPlane3D &res, const GPlane3D &p );

	// Constructs a plane from a point and a normal
	//GPlane3D& PlaneFromPointNormal( GPlane3D &res, const GVector3D &p, const GVector3D &n );


//------------------------------------------------------------------------------
//
// Matrix functions
//
//------------------------------------------------------------------------------
template<typename T, ::std::size_t N, ::std::size_t M>
	requires (N* M != 0) && NumericType<T>
MYMTL_NO_DISCARD constexpr MatrixNxM<T, N, M> zero_matrix() noexcept;


template<typename T, ::std::size_t N>
	requires (N > 0) && NumericType<T>
MYMTL_NO_DISCARD constexpr MatrixN<T, N> identity_matrix() noexcept;

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr MatrixN<T, 1> identity_matrix() noexcept;

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr MatrixN<T, 2> identity_matrix() noexcept;

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr MatrixN<T, 3> identity_matrix() noexcept;

template<typename T>
	requires NumericType<T>
MYMTL_NO_DISCARD constexpr MatrixN<T, 4> identity_matrix() noexcept;

template<typename T, typename U = T>
constexpr auto inverse_matrix(Matrix3<U>& res, U* det, const Matrix3<T>& m) noexcept;

//GMatrix3x3& MatrixIdentity( GMatrix3x3 &res );
//
//// Determines if a matrix is an identity matrix
//bool MatrixIsIdentity( const Matrix4 &m );
//bool MatrixIsIdentity( const GMatrix3x3 &m );

//// Returns the matrix transpose of a matrix
//Matrix4& MatrixTranspose( Matrix4 &res, const Matrix4 &m );
//GMatrix3x3& MatrixTranspose( GMatrix3x3 &res, const GMatrix3x3 &m );
//
//// Builds a matrix using the specified offsets
//Matrix4& MatrixTranslation( Matrix4 &res, gfloat x, gfloat y, gfloat z );
//GMatrix3x3& MatrixTranslation( GMatrix3x3 &res, gfloat x, gfloat y );

//// Builds a matrix that rotates around an arbitrary axis
//Matrix4& MatrixRotationAxis( Matrix4 &res, const GVector3D &v, gfloat angle );

//// Builds a matrix that rotates around the x-axis
//Matrix4& MatrixRotationX( Matrix4 &res, gfloat angle );

//// Builds a matrix that rotates around the y-axis
//Matrix4& MatrixRotationY( Matrix4 &res, gfloat angle );

//// Builds a matrix that rotates around the z-axis
//Matrix4& MatrixRotationZ( Matrix4 &res, gfloat angle );
//
//// Builds a matrix that rotates around the z-axis
//GMatrix3x3& MatrixRotation( GMatrix3x3 &res, gfloat angle );

//// Calculates the inverse of a matrix
////Matrix4& MatrixInverse( Matrix4 &res, gfloat *det, const Matrix4 &m );
//PGMatrix3x3 MatrixInverse( GMatrix3x3 &res, gfloat *det, const GMatrix3x3 &m );

//// Builds a matrix that scales along the x-axis, the y-axis, and the z-axis
//Matrix4& MatrixScaling( Matrix4 &res, gfloat sx, gfloat sy, gfloat sz );
//// Builds a matrix that scales along the x-axis, the y-axis
//GMatrix3x3& MatrixScaling( GMatrix3x3 &res, gfloat sx, gfloat sy );

//------------------------------------------------------------------------------
//
// Matrix functions
//
//------------------------------------------------------------------------------


////------------------------------------------------------------------------------
//	/* Функции для работы с кватернионами */
////------------------------------------------------------------------------------
//	CQuaternion StabilizeLength( const CQuaternion& q );
//	CQuaternion NormalizeQuaternion( const CQuaternion& q, gfloat Tolerance );
//	CQuaternion ConjugateQuaternion( const CQuaternion& q );
//	CQuaternion InverseQuaternion( const CQuaternion& q );
//	CQuaternion AxisAngleToQuaternion( const GVector3D& axis, gfloat angle );
//	CQuaternion YawPitchRollToQuaternion( gfloat Pitch, gfloat Yaw, gfloat Roll );
//	gfloat ScalarProduct( const CQuaternion& q1, const CQuaternion& q2 );
//	CQuaternion VectorProduct( const CQuaternion& q1, const CQuaternion& q2 );
//	CQuaternion VectorProduct2( const CQuaternion& q1, const CQuaternion& q2 );
//	CQuaternion VectorToQuaternion( const GVector3D& v );
//	void QuaternionToAxisAngle( const CQuaternion& q, GVector3D& axis, gfloat& angle );
//	CQuaternion RotationMatrixToUnitQuaternion( const CMatrix44& mat );
//
////------------------------------------------------------------------------------
//	/* Операторы для работы с кватернионами */
////------------------------------------------------------------------------------
//	CQuaternion operator * ( const CMatrix44& m, const CQuaternion& q );
//	CQuaternion operator * ( const CQuaternion& q, const CMatrix44& m );
//
////------------------------------------------------------------------------------
///* Функции для работы с матрицами */
////------------------------------------------------------------------------------
//	CMatrix44 TransposeMatrix( const CMatrix44& m );
//	CMatrix44 TranslationMatrixDX( gfloat x, gfloat y, gfloat z );
//	CMatrix44 TranslationMatrixGL( gfloat x, gfloat y, gfloat z );
//	CMatrix44 QuaternionToRotationMatrix( const CQuaternion& q );
//	CMatrix44 UnitQuaternionToRotationMatrix( const CQuaternion& q );
//	CMatrix44 VectorToDiagMatrix( const GVector3D& v );
//	CMatrix44 QuaternionToDiagMatrix( const CQuaternion& q );
////------------------------------------------------------------------------------
//
///**
//  class perform linear interpolation (approximate rotation interpolation),
//  result quaternion nonunit, its length lay between. sqrt(2)/2  and 1.0
//*/
//class APE_LYBAPI QLinearIntrp
//{
//public:
//    void SetQuaternions( const CQuaternion& _q1, const CQuaternion& _q2, bool UnitLength );
//	CQuaternion Interpolate( gfloat t );
//private:
//	CQuaternion  q1_; 
//	CQuaternion	 q2_;
//};
////------------------------------------------------------------------------------
///**
// Perform Spherical Linear Interpolation of the quaternions,
// return unit length quaternion 
//*/
//class APE_LYBAPI QSLinearIntrp
//{
//public:
//    void SetQuaternions( const CQuaternion& _q1, const CQuaternion& _q2, bool UnitLength );
//    CQuaternion Interpolate( gfloat t );
//
//private:
//	CQuaternion q1_, q2_;
//    gfloat omega_;
//};

MYMTL_NS_END

#include "ApeMath-inl.h"

#endif //ApeMath_h