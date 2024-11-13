#ifndef __Plane_inl__
#define __Plane_inl__

#include "Plane.h"


inline GPlane3D::GPlane3D( gfloat initA, gfloat initB, gfloat initC, gfloat initD )
	: a( initA ), b( initB ), c( initC ), d( initD )
{}

inline GPlane3D::GPlane3D( const GVector3D &initABC, gfloat initD )
	: n( initABC ), d( initD )
{}

// Creates vector from C array
inline GPlane3D::GPlane3D( const gfloat *pv )
{
#ifdef _DEBUG
	if ( !pv )
		return;
#endif
	a = pv[0];
	b = pv[1];
	c = pv[2];
	d = pv[3];
}


// Copy constructor
inline GPlane3D::GPlane3D( const GPlane3D &src )
	: a( src.a ), b( src.b ), c( src.c ), d( src.d )
{}



// Assignment operator
inline GPlane3D& GPlane3D::operator = ( const GPlane3D &rhs )
{
	if ( &rhs != this )
	{
		a = rhs.a;
		b = rhs.b;
		c = rhs.c;
		d = rhs.d;
	}
	return *this;
}


// Creates opposite directed plane
inline GPlane3D GPlane3D::operator -() const
{
	return GPlane3D( -a, -b, -c, -d );
}


inline GPlane3D::operator gfloat* ()
{
	return ( gfloat* ) &a;
}


inline GPlane3D::operator const gfloat* () const
{
	return ( const gfloat* ) &a;
}


inline const GPlane3D& GPlane3D::operator *=( gfloat value )
{
	a *= value;
	b *= value;
	c *= value;
	d *= value;

	return *this;
}


inline const GPlane3D& GPlane3D::operator /=( gfloat value )
{
	gfloat valueInv = ( gfloat )1.0 / value;
	
	a *= valueInv;
	b *= valueInv;
	c *= valueInv;
	d *= valueInv;

	return *this;
}

//------------------------------------------------------------------------------
// Aux operators
//------------------------------------------------------------------------------
inline bool operator ==( const GPlane3D &p1, const GPlane3D &p2 )
{
	return ( ( p1.a == p2.a ) && ( p1.b == p2.b ) && ( p1.c == p2.c ) && ( p1.d == p2.d ) );
}

inline bool operator !=( const GPlane3D &p1, const GPlane3D &p2 )
{
	return ( ( p1.a != p2.a ) || ( p1.b != p2.b ) || ( p1.c != p2.c ) || ( p1.d != p2.d ) );
}


inline GPlane3D operator * ( const GPlane3D &lhs, gfloat value )
{
	return GPlane3D( lhs.a * value, lhs.b * value, lhs.c * value, lhs.d * value );
}

inline GPlane3D operator * ( gfloat value, const GPlane3D &rhs )
{
	return GPlane3D( value * rhs.a, value * rhs.b, value * rhs.c, value * rhs.d );
}

inline GPlane3D operator / ( const GPlane3D &lhs, gfloat value )
{
	gfloat valueInv = ( gfloat )1.0 / value;
	return GPlane3D( lhs.a * valueInv, lhs.b * valueInv, lhs.c * valueInv, lhs.d * valueInv );
}


#endif //__APE_Plane_inl__

