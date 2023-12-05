#include <algorithm>
#include <cmath>
#include <limits>

#include <raylib.h>
#include <raymath.h>
#include <brutus/brutus.h>

#include "sdf.hh"

using namespace std;

// Primatives
SDF space() {
	return [=](vec3 p) {
		return std::numeric_limits<float>::infinity();
	};
}

SDF sphere(float radius) {
	return [=](vec3 p) {
		return p.Length() - radius;
	};
}

SDF box(float x, float y, float z) {
	return [=](vec3 p) {
		vec3 q = vec3( abs(p.x), abs(p.y), abs(p.z) ) - vec3(x,y,z);
		return q.Max( vec3::Zero() ).Length() + fmin( fmax( q.x, fmax(q.y, q.z) ), 0.0 );
		// return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
	};
}

// Operations
SDF transform(SDF f, mat4 m) {
	return [=](vec3 p) {
		return f( p.Transform( m.Invert() ) );
	};
}

SDF add(SDF a, SDF b) {
	return [=](vec3 p) {
		return min( a(p), b(p) );
	};
}

SDF sub(SDF a, SDF b) {
	return [=](vec3 p) {
		return max( -a(p), b(p) );
	};
}


SDF add(SDF a, SDF b, float k) {
	return [=](vec3 p) {
		const float A = a(p);
		const float B = b(p);

		float h = clamp( 0.5 + 0.5*(B-A)/k, 0.0, 1.0 );
		return lerp( B, A, h ) - k * h * (1.0-h);
	};
}

SDF sub(SDF a, SDF b, float k) {
	return [=](vec3 p) {
		const float A = a(p);
		const float B = b(p);

		float h = clamp( 0.5 - 0.5*(B+A)/k, 0.0, 1.0 );
		return lerp( B, -A, h ) + k*h*(1.0-h);
	};
}
