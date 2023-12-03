#include <brutus/brutus.h>
#include <raylib-cpp.hpp>

#include "planet.hh"

const float r = (Brutus::Chunk::size / 2) - 1;

SDF moon() {
	float radius = GetRandomValue(4, r);
	SDF planet = sphere(radius);

	int n = GetRandomValue(10, 100);
	for (int i = 0; i < n; i++) {
		float s = GetRandomValue(1, radius / 2);
		SDF crater = sphere(s);
		crater = transform( crater, mat4::Translate(radius + s / 2, 0, 0) );
		vec3 rotation( GetRandomValue(0, 360), GetRandomValue(0, 360), GetRandomValue(0, 360) );
		crater = transform( crater, mat4::RotateXYZ(rotation) );

		planet = sub(crater, planet, 0.25);
	}

	planet = transform( planet, mat4::Translate(r, r, r) );

	return planet;
}
