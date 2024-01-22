#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    // Rays are composed of a starting point and a direction vector.
    // The ray is parameterized with 't':
    // ray = origin + t * direction
    public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const {
        // Return coordintes of ray parameteri
        return orig + t*dir;
    }

    private:
    point3 orig;
    vec3 dir;
};


#endif
