#pragma once

#include <fstream>

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"


class camera {
public:
    /* Public Camera Parameters Here */
	double aspect_ratio = 16.0 / 9.0;	// Ratio of image width over height
	int image_width = 400;				// Rendered image width in pixel count
	int samples_per_pixel = 10;			// Count of random samples for each pixel
	int max_depth = 10;					// Maximum number of ray bounces into scene
	double diffuse_reflectance = 0.5;	// Reflectance percentage of diffuse material

    void render(const hittable& world, std::string filename) {

		initialize();

		std::ofstream outFile(filename);

		if (!outFile.is_open()) {
			std::cerr << "Error: could not open the file for writing." << std::endl;
			return;
		}

		// P3: ASCII colors
		// Next two numbers: columns, rows
		// Next number: max color value
		outFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		// Colors as RGB triplets
		for (int j = 0; j < image_height; j++) {

			// Log progress
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				
				for (int k = 0; k < samples_per_pixel; k++) {
					pixel_color += ray_color(get_ray(i, j), max_depth, world);
				}

				pixel_color *= 1.0 / samples_per_pixel;

				write_color(outFile, pixel_color);
			}
		}

		std::clog << "\rDone.                 \n";

		// Close the file when done
		outFile.close();

		// Check for any write errors
		if (outFile.fail()) {
			std::cerr << "Error: Failed to write to the file." << std::endl;
			return;
		}

		std::cout << "Data has been written to " << filename << std::endl;
    }

private:
    /* Private Camera Variables Here */
	int image_height;    // Rendered image height
	point3 camera_center;       // Camera center
	point3 pixel00_loc;  // Location of pixel 0, 0
	vec3 pixel_delta_u;  // Offset to pixel to the right
	vec3 pixel_delta_v;  // Offset to pixel below

    void initialize() {
		double focal_length = 1.0;
		double viewport_height = 2.0;

		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		// We don't use aspect_ratio because aspect_ratio used real-valued numbers and in actual image width and height we rounded
		double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		camera_center = point3(0, 0, 0);

		// Calculate vectors along the horizontal and down the vertical viewport edges.
		vec3 viewport_u = vec3(viewport_width, 0, 0);
		vec3 viewport_v = vec3(0, -viewport_height, 0);
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate location of upper left corner of the viewport. Subtracting focal length points us further into the screen.
		point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

		// The location of the upper left pixel should be in the center of its grid square, not on a viewport corner.
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

	color ray_color(const ray& r, int depth, const hittable& world) {
		hit_record rec;

		// If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0) return color(0, 0, 0);

		// Floating point rounding can result in issues such as a ray with origin just below the surface of the sphere,
		// originated from a bounce off the surface of the sphere, that then goes on to intersect the inside of the sphere
		// immediately. To avoid this we specify we must have traveled for at least some non-neglible t to count the intersection.
		if (world.hit(r, interval(0.001, infinity), rec)) {
			vec3 direction = rec.normal + random_on_unit_sphere();
			return diffuse_reflectance * ray_color(ray(rec.p, direction), depth - 1, world);
		}

		// normalize y-values to range [0,1]
		vec3 ray_dir = (unit_vector(r.direction()) + vec3(0, 1, 0)) / 2;
		color ray_color = (1 - ray_dir.y) * color(1, 1, 1) + ray_dir.y * color(0.5, 0.7, 1.0);
		return ray_color;
	}

	ray get_ray(int i, int j) const {
		// Get a randomly sampled camera ray for the pixel at location i,j.
		point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		point3 pixel_sample = pixel_center + pixel_sample_square();

		vec3 ray_direction = pixel_sample - camera_center;

		return ray(camera_center, ray_direction);
	}

	vec3 pixel_sample_square() const {
		// Returns a random point in the square surrounding a pixel at the origin.
		double px = -0.5 + random_double();
		double py = -0.5 + random_double();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}
};
