#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public: 
	union {
		struct { double x, y, z; };
		struct { double r, g, b; };
		double e[3];
	};
		
	vec3() : e{ 0, 0, 0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// get value
	double operator[](int i) const { return e[i]; }

	// set value
	double& operator[](int i) { return e[i]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(double t) {
		return *this *= 1 / t;
	}

	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double length() const {
		return sqrt(length_squared());
	}

	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

// Alias for clarity
using point3 = vec3;

// Utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

// elementwise multiplication
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return v * t;
}

inline vec3 operator/(const vec3& v, double t) {
	return 1 / t * v;
}

inline double dot(const vec3& u, const vec3& v) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline vec3 random_on_unit_sphere() {
	while (true) {
		vec3 p = vec3::random(-1, 1);
		if (p.length_squared() < 1) {
			return unit_vector(p);
		}
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_on_unit_sphere();
	// If the dot product is negative we are on the wrong hemisphere, but can get onto the right one by inverting the surface normal.
	return (dot(on_unit_sphere, normal) > 0.0) ? on_unit_sphere : -on_unit_sphere;
}


