#pragma once

#include "rtweekend.h"
#include "hittable_list.h"
#include "color.h"

class material {
public: 
	virtual ~material() = default;

	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
private:
	color albedo;

public:
	lambertian(const color& a) : albedo(a) {}
	
	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 scatter_direction = rec.normal + random_on_unit_sphere();

		// Catch degenerate scatter direction case
		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
private: 
	color albedo;
	double fuzz;

public:
	metal(const color& a, double f) : albedo(a), fuzz(f) {}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflection = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = ray(rec.p, reflection + fuzz * random_on_unit_sphere());
		attenuation = albedo;
		return true;
	}

};