#pragma once

#include <functional>

#include "typedefs.hh"

// namespace SDF {
//
// void sphere(Brutus::Grid& grid, Vector3 center, float radius, int polarity);
//
// }

typedef std::function< float(vec3) > SDF;

SDF sphere(float radius);
