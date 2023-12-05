#include <random>

#include <brutus/brutus.h>
#include <raylib-cpp.hpp>

#include "planet.hh"

const float r = Brutus::Chunk::size / 2;
const float min_radius = 7;
const float max_radius = 12;
const float crater_coeff = 1.0;
const int asteroid_count = 2;
const float asteroid_scale = 0.8;

std::random_device rd;
std::mt19937 mt( rd() );
std::uniform_real_distribution<float> planet_size(min_radius, max_radius);
std::uniform_real_distribution<float> crater_size(3, 5);
std::uniform_real_distribution<float> crater_angle(0, 360);
std::uniform_real_distribution<float> asteroid_offset(-min_radius, min_radius);

SDF add_craters(SDF func, float radius) {
	SDF craters = space();
	int n = radius * crater_coeff;
	for (int i = 0; i < n; i++) {
		float s = crater_size(mt);
		SDF crater = sphere(s);
		crater = transform( crater, mat4::Translate(radius + 1, 0, 0) );
		vec3 rotation( crater_angle(mt), crater_angle(mt), crater_angle(mt) );
		crater = transform( crater, mat4::RotateXYZ(rotation) );

		craters = add(craters, crater);
	}

	func = add(craters, func, 4.0);
	func = sub( transform( craters, mat4::Scale(1.1, 1.1, 1.1) ), func, 1.0 );

	return func;
}

SDF moon() {
	float radius = planet_size(mt);
	SDF planet = sphere(radius);

	planet = add_craters(planet, radius);
	planet = transform( planet, mat4::Translate(r, r, r) );

	return planet;
}

SDF asteroid() {
	SDF planet = sphere(1);

	for (int i = 0; i < asteroid_count; i++) {
		SDF m = moon();

		m = transform( m, mat4::Translate(-r, -r, -r) );

		vec3 offset( asteroid_offset(mt), asteroid_offset(mt), asteroid_offset(mt) );
		m = transform( m, mat4::Translate(offset.x, offset.y, offset.z) );

		planet = add(planet, m, 8.0);
	}

	planet = transform(planet, mat4::Scale(asteroid_scale, asteroid_scale, asteroid_scale) );
	planet = transform( planet, mat4::Translate(r, r, r) );

	return planet;
}
