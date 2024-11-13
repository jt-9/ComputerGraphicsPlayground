#ifndef Rasteriser_h
#define Rasteriser_h

// Defines policy which is used to draw pixels, lines, curves. CoordSpace delegates actual drawing to the policy

#include "CoordSpaceDefs.h"

#ifdef _WINDOWS
	#include "WinRasterisationPolicies.h"
	#include <concepts>

MY_COORD_SPACE_BEGIN
	using Rasteriser = ::coordsys::WinGDIRasteriser;
MY_COORD_SPACE_END
#elif defined __linux__
#endif


#endif //!Rasteriser_h