#ifndef __APE_Quaternion_inl__
#define __APE_Quaternion_inl__

#include "Quaternion.h"

APE_NS_BEGIN


inline CQuaternion::CQuaternion()
{
	x = y = z = 0;
	w = 1;
}

inline CQuaternion::CQuaternion( Ftype _x, Ftype _y, Ftype _z, Ftype _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

inline CQuaternion::CQuaternion( const CQuaternion& src )
{
	x = src.x;
	y = src.y;
	z = src.z;
	w = src.w;
}

inline void CQuaternion::Idential()
{
	x = y = z = 0; 
	w = 1;
}

inline const CQuaternion& CQuaternion::operator = ( const CQuaternion& rhs )
{
	if ( this != &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}
	return *this; 
}  

inline CQuaternion CQuaternion::operator - () const
{
	return CQuaternion( -x, -y, -z, -w );
}

inline const CQuaternion& CQuaternion::operator += ( const CQuaternion& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

inline const CQuaternion& CQuaternion::operator -= ( const CQuaternion& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

inline const CQuaternion& CQuaternion::operator *= ( Ftype value )
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;

	return *this;
}

inline const CQuaternion& CQuaternion::operator /= ( Ftype value )
{
	Ftype valueInv = 1.0f / value;
	x *= valueInv;
	y *= valueInv;
	z *= valueInv;
	w *= valueInv;

	return *this;
}

inline Ftype& CQuaternion::operator [] ( byte Index )
{
	return *( &x + Index );
} 

inline Ftype CQuaternion ::operator [] ( byte Index ) const
{
	return *( &x + Index );
}

//------------------------------------------------------------------------------
//Дружественные операторы
//------------------------------------------------------------------------------
inline bool operator == ( const CQuaternion &lhs, const CQuaternion &rhs )
{
	return ( ( lhs.x == rhs.x )&&( lhs.y == rhs.y )&&( lhs.z == rhs.z )&&( lhs.w == rhs.w ) );
}
//------------------------------------------------------------------------------
inline bool operator != ( const CQuaternion &lhs, const CQuaternion &rhs )
{
	return ( ( lhs.x != rhs.x )||( lhs.y != rhs.y )||( lhs.z != rhs.z )||( lhs.w != rhs.w ) );
}
//------------------------------------------------------------------------------
inline CQuaternion	operator * ( const CQuaternion &lhs, Ftype value )
{
	return CQuaternion( lhs.x * value, lhs.y * value, lhs.z * value, lhs.w * value );
}
//------------------------------------------------------------------------------
inline CQuaternion	operator * ( Ftype value, const CQuaternion &rhs )
{
	return CQuaternion( value * rhs.x, value * rhs.y, value * rhs.z, value * rhs.w );
}
//------------------------------------------------------------------------------
inline CQuaternion	operator / ( const CQuaternion &lhs, Ftype value )
{
	Ftype valueInv = 1.0f / value;
	return CQuaternion( lhs.x * valueInv, lhs.y * valueInv, lhs.z * valueInv, lhs.w * valueInv );
}
//------------------------------------------------------------------------------
inline CQuaternion	operator + ( const CQuaternion& lhs, const CQuaternion& rhs )
{
	return CQuaternion( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w );
}
//------------------------------------------------------------------------------
inline CQuaternion	operator - ( const CQuaternion& lhs, const CQuaternion& rhs )
{
	return CQuaternion( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w );
}

//------------------------------------------------------------------------------
	/* Функции для работы с кватернионами */
//------------------------------------------------------------------------------
inline bool IsEqual( const CQuaternion& q1, const CQuaternion& q2, Ftype Tolerance )
{
	return (  ( fabs( q1.x - q2.x ) < Tolerance )
			&&( fabs( q1.y - q2.y ) < Tolerance )
			&&( fabs( q1.z - q2.z ) < Tolerance )
			&&( fabs( q1.w - q2.w ) < Tolerance ) );
}
//------------------------------------------------------------------------------
inline bool IsNull( const CQuaternion& q, Ftype Tolerance )
{
	return ( ( q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w ) < Tolerance*Tolerance );
}

APE_NS_END

#endif //__APE_Quaternion_inl__