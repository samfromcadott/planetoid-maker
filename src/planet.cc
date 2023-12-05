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

typedef std::uniform_real_distribution<float> Dist;

Dist planet_size(min_radius, max_radius);
Dist crater_size(3, 5);
Dist crater_angle(0, 360);
Dist asteroid_offset(-min_radius, min_radius);
Dist eye_size(0.8, 1.2);
Dist nose_size(1.0, 2.0);
Dist chin_size(0.5, 2.0);

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

SDF mirror(SDF f) {
	return [=](vec3 p) {
		p.y = abs(p.y);
		return f(p);
	};
}

SDF head() {
	float radius = planet_size(mt);

	// Cranium
	SDF cranium = sphere(radius);
	cranium = transform(cranium, mat4::Scale(1.0, 0.9, 1.0) );

	// Jaw
	SDF jaw = sphere( radius/8  * chin_size(mt) );
	jaw = transform( jaw, mat4::Translate( radius / 2, 0, -radius ) );

	// Eye sockets
	float socket_size = radius / 5 * eye_size(mt);
	SDF sockets = sphere(socket_size);
	sockets = transform(sockets, mat4::Scale(1.0, 1.5, 1.0));
	sockets = transform( sockets, mat4::Translate(radius - socket_size, radius - socket_size * 2, 0) );
	sockets = mirror(sockets);

	// Nose
	SDF nose = sphere( radius / 10 * nose_size(mt) );
	nose = transform( nose, mat4::Translate(radius, 0, -4) );

	// // Lips
	// SDF lips = sphere( radius / 10 * nose_size(mt) );
	// lips = transform(lips, mat4::Scale(0.5, 3.0, 1.0));
	// lips = transform( lips, mat4::Translate(radius - 1, 0, -radius + 2) );

	// Put it all together
	SDF planet = add(cranium, jaw, radius);
	// planet = add_craters(planet, radius);
	planet = sub(sockets, planet, 2.0);
	planet = add(planet, nose, 3.0);
	// planet = add(planet, lips, 5.0);

	vec3 rotation( crater_angle(mt), crater_angle(mt), crater_angle(mt) );
	planet = transform( planet, mat4::RotateXYZ(rotation) );
	planet = transform( planet, mat4::Translate(r, r, r) );

	return planet;
}
