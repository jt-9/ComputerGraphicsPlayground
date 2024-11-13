#ifndef __Plane_h__
#define __Plane_h__

#include "../defs.h"
#include "Vector.h"

APE_NS_BEGIN


// Class incapsulates primitive plane operations
struct GPlane3D
{
public:

	GPlane3D() {};
	
	GPlane3D( gfloat initA, gfloat initB, gfloat initC, gfloat initD );
	GPlane3D( const GVector3D &initABC, gfloat initD );
	GPlane3D( const gfloat *pv );
	GPlane3D( const GPlane3D &src );

	GPlane3D& operator = ( const GPlane3D &rhs );

	GPlane3D operator - ( void ) const;
	
	operator gfloat* ();
	operator const gfloat* () const;
	
	const GPlane3D&	operator *= ( gfloat value );
	const GPlane3D&	operator /=	( gfloat value );

	union
	{
		struct
		{
			gfloat a;
			gfloat b;
			gfloat c;
		};
		GNORMAL3D n;
	};
	gfloat d;
}; 


//------------------------------------------------------------------------------
	/* Aux operators */
//------------------------------------------------------------------------------
	bool operator ==( const GPlane3D &v1, const GPlane3D &v2 );
	bool operator !=( const GPlane3D &v1, const GPlane3D &v2 );

	//bool IsEqual( const VECTOR3D& v1, const VECTOR3D& v2, gfloat Tolerance );
	//bool IsNull( const VECTOR3D& v, gfloat Tolerance );

	GPlane3D	operator *  ( const GPlane3D &lhs, gfloat value );
	GPlane3D	operator *  ( gfloat value, const GPlane3D &rhs );
	GPlane3D	operator /  ( const GPlane3D &lhs, gfloat value );

//------------------------------------------------------------------------------

#include "Plane.inl"

	
APE_NS_END

#endif // __Plane_h__