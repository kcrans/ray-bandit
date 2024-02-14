#ifndef RTCOMMON_H
#define RTCOMMON_H

#include <cmath>
#include <cstdlib>
// #include <random>
#include <limits>
#include <memory>

// using-directives for namespaces

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// utility functions
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	// return a random real in [0, 1)
   // static std::uniform_real_distribution<double> distribution(0.0, 1.0);
   // static std::mt19937 generator;
   // return distribution(generator);
   return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// return a random real in [min, max)
	return min + (max - min)*random_double();
}

// shared headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
