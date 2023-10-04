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

class dielectric : public material {
private:
	double ir; // Index of refraction

	static double reflectance(double cosine, double refraction_ratio) {
		// Use Schlick's approximation for reflectance.
		double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		double refraction_ratio = (rec.front_face) ? (1.0 / ir) : ir;

		vec3 unit_dir = unit_vector(ray_in.direction());
		double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
			direction = reflect(unit_dir, rec.normal);
		}
		else {
			direction = refract(unit_dir, rec.normal, refraction_ratio);
		}
		
		scattered = ray(rec.p, direction);
		attenuation = color(1.0, 1.0, 1.0);
		return true;
	}
};