#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "scene_objects.h"
#include "vec3.h"

class triangle : public scene_object {
    public:
    triangle(point3 _vertex_a, point3 _vertex_b, point3 _vertex_c, vec3 _normal, shared_ptr<material> _material) : vertex_a(_vertex_a), vertex_b(_vertex_b), vertex_c(_vertex_c), normal(_normal),  mat(_material) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto t =;

        // Does the ray intersect the plane in which the triangle is situated?
        vec3 denom = dot(r.direction(), normal);
        vec3 numer = dot((vertex_a - r.origin()), normal);
        if (denom == 0) {
            return false; // line and plane are parallel
                          // which we reject even if they coincide
        }
        // Does the point of intersection lie within the triangle's sides?
        auto t = numer / denom;
        vec3 intersect_point = r.at(t);

        if (dot((cross(vertex_b - vertex_a, intersect_point - vertex_a)), normal) < 0)
            return false;
        if (dot((cross(vertex_c - vertex_c, intersect_point - vertex_b)), normal) < 0)
            return false;
         if (dot((cross(vertex_a - vertex_c, intersect_point - vertex_c)), normal) < 0)
            return false;       
        rec.t = t;
        rec.p = intersect_point;
        vec3 outward_normal = (rec.p - center) / radius; // divide by radius for unit length
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    private:
    point3 vertex_a;
    point3_vertex_b;
    point3 vertex_c;
    vec3   normal;
    shared_ptr<material> mat;

};

#endif
