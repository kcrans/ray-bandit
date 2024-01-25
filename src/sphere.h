#ifndef SPHERE_H
#define SPHERE_H

#include "scene_objects.h"
#include "vec3.h"

class sphere : public scene_object {
    public:
    sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root between t_min and t_max
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false; // both roots out of range
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius; // divide by radius for unit length
        rec.set_face_normal(r, outward_normal);

        return true;
    }

    private:
    point3 center;
    double radius;


};

#endif
