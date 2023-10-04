#pragma once

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
private:
	point3 center;
	double radius;
	shared_ptr<material> mat;

public: 
	sphere(point3 _center, double _radius, shared_ptr<material> _mat) : center(_center), radius(_radius), mat(_mat) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		double a = dot(r.direction(), r.direction());
		double half_b = dot(r.direction(), r.origin() - center);
		double c = dot(r.origin() - center, r.origin() - center) - radius * radius;
			
		double discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;

		double sqrt_d = sqrt(discriminant);
		double root = (-half_b - sqrt_d) / a;
		if (!ray_t.surrounds(root)) {
			root = (-half_b + sqrt_d) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t); 
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;
		return true;
	}
};