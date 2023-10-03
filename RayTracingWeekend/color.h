#pragma once

#include <iostream>

#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear_component) {
	return sqrt(linear_component);
}

void write_color(std::ofstream& outFile, color& pixel_color) {
	static const interval intensity = interval(0, 0.999);

	double r = 256 * intensity.clamp(linear_to_gamma(pixel_color.r));
	double g = 256 * intensity.clamp(linear_to_gamma(pixel_color.g));
	double b = 256 * intensity.clamp(linear_to_gamma(pixel_color.b));

	// Write translated [0,255] value of each color component
	outFile 
		<< static_cast<int>(r) << ' '
		<< static_cast<int>(g) << ' '
		<< static_cast<int>(b) << '\n';
}