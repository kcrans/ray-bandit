#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "common.h"

using std::sqrt;

class vec3 {
    public:
        double e[3];
        
        vec3() : e{0, 0, 0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3 &v) {
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
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        bool near_zero() const {
            // Returns true if the vector is near (0, 0, 0)
            // I.e. if each of its components is within some epsilon of zero
            // Technically we are using the unifrom (sup) norm
            auto epsilon = 1e-8;
            return (fabs(e[0]) < epsilon) && (fabs(e[1]) < epsilon) && (fabs(e[2]) < epsilon);
        }

	    static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
	}

	    static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

// alias for vec3 used when dealing with points explicitly
using point3 = vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3 &v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    // keep on generating random vectors until one is in the unit disk
    // on the x-y plane
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    } 
}

inline vec3 random_in_unit_sphere() {
    // keep on generating random vectors until one is in the unit sphere
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // Points same direction as normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n)*n;
}

inline vec3 refract(const vec3& v, const vec3& n, double refract_index_ratio) {
    auto cos_theta = fmin(dot(v, -n), 1.0);
    auto sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    
    // Calculates reflectance coeeficents using Schlick's approximation
    auto R0 = pow((1 - refract_index_ratio) / (1 + refract_index_ratio), 2);
    auto R1 =  R0 + (1 - R0) * pow((1 - cos_theta), 5);

    if ((sin_theta * refract_index_ratio > 1.0) || R1 > random_double()) {
        return reflect(v, n);
    }
    vec3 r_perp = refract_index_ratio * (v + cos_theta*n);
    vec3 r_para = -1 * sqrt(1.0 - r_perp.length_squared()) * n;
    if (std::isnan(r_para.x())) {
        std::cout << "[" <<  r_perp.length_squared() << " " << v.length_squared() << " " << (-n).length_squared() << "]" << std::endl;
    }

    return r_perp + r_para;
}

#endif
