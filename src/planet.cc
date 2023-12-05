#include <random>

#include <brutus/brutus.h>
#include <raylib-cpp.hpp>

#include "planet.hh"

const float r = Brutus::Chunk::size / 2;
const float min_radius = 10;
const float max_radius = 12;
const float crater_coeff = 1;
const int asteroid_count = 3;

std::random_device rd;
std::mt19937 mt( rd() );
std::uniform_real_distribution<float> planet_size(min_radius, max_radius);
std::uniform_real_distribution<float> crater_size(3, 5);
std::uniform_real_distribution<float> crater_angle(0, 360);

SDF moon() {
	float radius = planet_size(mt);
	SDF planet = sphere(radius);

	SDF craters = sphere(0);
	int n = radius * crater_coeff;
	for (int i = 0; i < n; i++) {
		float s = crater_size(mt);
		SDF crater = sphere(s);
		crater = transform( crater, mat4::Translate(radius + 1, 0, 0) );
		vec3 rotation( crater_angle(mt), crater_angle(mt), crater_angle(mt) );
		crater = transform( crater, mat4::RotateXYZ(rotation) );

		craters = add(craters, crater);
	}

	planet = add(craters, planet, 3.0);
	planet = sub( transform( craters, mat4::Scale(1.1, 1.1, 1.1) ), planet, 1.0 );

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
