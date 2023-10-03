#pragma once

#include "rtweekend.h"

class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(double _min, double _max) : min(_min), max(_max) {}

    // closed interval
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // open interval
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        return (x < min) ? min : (x > max) ? max : x;
    }

    static const interval empty, universe;
};

static const interval empty(+infinity, -infinity);
static const interval universe(-infinity, +infinity);