#pragma once

#include <functional>

#include "typedefs.hh"

// namespace SDF {
//
// void sphere(Brutus::Grid& grid, Vector3 center, float radius, int polarity);
//
// }

typedef std::function< float(vec3) > SDF;

// Primatives
SDF sphere(float radius);
SDF box(float x, float y, float z);

// Operations
SDF transform(SDF f, mat4 m);
SDF add(SDF a, SDF b);
SDF sub(SDF a, SDF b);
