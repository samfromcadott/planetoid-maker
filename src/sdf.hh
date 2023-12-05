#pragma once

#include <functional>

#include "typedefs.hh"

typedef std::function< float(vec3) > SDF;

// Primatives
SDF space();
SDF sphere(float radius);
SDF box(float x, float y, float z);

// Operations
SDF transform(SDF f, mat4 m);
SDF add(SDF a, SDF b);
SDF sub(SDF a, SDF b);
SDF add(SDF a, SDF b, float k);
SDF sub(SDF a, SDF b, float k);
