#ifndef COORD_SYSTEM_SHARED_MODELS_HPP
#define COORD_SYSTEM_SHARED_MODELS_HPP

#pragma once

#include "CartesianCoordSys2D.h"
#include "LabelFormatter.hpp"
    
using CoordSys2D = coordsys::CartesianCoordSys2D<int, float, double, coordsys::LabelFormatter<float>>;

#endif