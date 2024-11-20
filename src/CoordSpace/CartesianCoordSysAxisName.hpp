#ifndef CARTESIAN_COORD_SYS_AXIS_NAME_HPP
#define CARTESIAN_COORD_SYS_AXIS_NAME_HPP

#pragma once

#include "CoordSpaceDefs.h"

#include <cstdint>

MY_COORD_SPACE_BEGIN

enum class AxisName : std::int8_t
{
    Abscissa = 0, Ordinate, Applicate
};

MY_COORD_SPACE_END

#endif // !CARTESIAN_COORD_SYS_AXIS_NAME_HPP
