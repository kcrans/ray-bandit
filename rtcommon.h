#ifndef RTCOMMON_H
#define RTCOMMON_H

#include <cmath>
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

// shared headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
