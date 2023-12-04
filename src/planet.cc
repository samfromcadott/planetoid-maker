#include <brutus/brutus.h>
#include <raylib-cpp.hpp>

#include "planet.hh"

const float r = (Brutus::Chunk::size / 2) - 1;
const float min_radius = 12;
const float crater_coeff = 1;
const int asteroid_count = 3;

SDF moon() {
	float radius = GetRandomValue(min_radius, r-1);
	SDF planet = sphere(radius);

	int n = radius * crater_coeff;
	for (int i = 0; i < n; i++) {
		float s = GetRandomValue(6, radius / 12);
		SDF crater = sphere(s);
		crater = transform( crater, mat4::Translate(radius + 1, 0, 0) );
		vec3 rotation( GetRandomValue(0, 360), GetRandomValue(0, 360), GetRandomValue(0, 360) );
		crater = transform( crater, mat4::RotateXYZ(rotation) );

		planet = add(crater, planet, 4.0);
		planet = sub( transform( crater, mat4::Scale(1.1, 1.1, 1.1) ), planet, 2.0 );
	}

	planet = transform( planet, mat4::Translate(r, r, r) );

	return planet;
}

SDF asteroid() {
	SDF planet = sphere(1);

	for (int i = 0; i < asteroid_count; i++) {
		SDF m = moon();

		m = transform( m, mat4::Translate(-r, -r, -r) );
		m = transform( m , mat4::Scale(0.5, 0.5, 0.5) );

		vec3 d( GetRandomValue(0, min_radius), GetRandomValue(0, min_radius), GetRandomValue(0, min_radius) );
		m = transform( m, mat4::Translate(d.x, d.y, d.z) );

		planet = add(planet, m, 16.0);
	}

	planet = transform( planet, mat4::Translate(r/2, r/2, r/2) );

	return planet;
}
