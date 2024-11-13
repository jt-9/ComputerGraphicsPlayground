#ifndef __APE_Quaternion_h__
#define __APE_Quaternion_h__

#include "ApeCommon.h"

APE_NS_BEGIN

struct APE_LYBAPI CQuaternion
{
public:
    CQuaternion();
    CQuaternion( Ftype _x, Ftype _y, Ftype _z, Ftype _w );
	CQuaternion( const CQuaternion& src );
    ~CQuaternion() {};    
    
	const CQuaternion&	operator = ( const CQuaternion& rhs );
    
	void Idential();
	
	CQuaternion			operator - () const;
    const CQuaternion&	operator += ( const CQuaternion& rhs );
    const CQuaternion&	operator -= ( const CQuaternion& rhs );
    const CQuaternion&	operator *= ( Ftype value );
	const CQuaternion&	operator /= ( Ftype value );
	Ftype&				operator [] ( byte Index );
	Ftype				operator [] ( byte Index ) const;

public:
	Ftype x;
	Ftype y;
	Ftype z;
	Ftype w;
};

//------------------------------------------------------------------------------
/* Дружественные операторы */
//------------------------------------------------------------------------------
	bool operator ==( const CQuaternion &lhs, const CQuaternion &rhs );
	bool operator !=( const CQuaternion &lhs, const CQuaternion &rhs );

	bool IsEqual( const CQuaternion& q1, const CQuaternion& q2, Ftype Tolerance );
	bool IsNull( const CQuaternion& q, Ftype Tolerance );

	CQuaternion	operator * ( const CQuaternion &lhs, Ftype value );
	CQuaternion	operator * ( Ftype value, const CQuaternion &rhs );
	CQuaternion	operator / ( const CQuaternion &lhs, Ftype value );

	CQuaternion	operator + ( const CQuaternion& lhs, const CQuaternion& rhs );
	CQuaternion	operator - ( const CQuaternion& lhs, const CQuaternion& rhs );
//------------------------------------------------------------------------------

APE_NS_END

#include "Quaternion.inl" 

#endif //__APE_Quaternion_h__
