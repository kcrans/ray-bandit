#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "scene_objects.h"
#include "vec3.h"

class triangle : public scene_object {
    public:
    triangle(point3 v_a, point3 v_b, point3 v_c, vec3 n, shared_ptr<material> _mat) : vertex_a(v_a), vertex_b(v_b), vertex_c(v_c), normal(n), mat(_mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

        // Does the ray intersect the plane in which the triangle is situated?
        auto denom = dot(r.direction(), normal);
        auto numer = dot((vertex_a - r.origin()), normal);
        if (denom <= 0.01 & denom >= -0.01) {
            return false; // line and plane are parallel
                          // which we reject even if they coincide
        }
        // Does the point of intersection lie within the triangle's sides?
        auto t = numer / denom;
        if (!ray_t.surrounds(t)) {
            return false; 
        }
        vec3 intersect_point = r.at(t);

        if (dot((cross(vertex_b - vertex_a, intersect_point - vertex_a)), normal) < 0)
            return false;
        if (dot((cross(vertex_c - vertex_b, intersect_point - vertex_b)), normal) < 0)
            return false;
        if (dot((cross(vertex_a - vertex_c, intersect_point - vertex_c)), normal) < 0)
            return false;       

        // std::cout << normal << " + " << intersect_point << std::endl;        
        // We now know the ray intersects the triangle
        rec.t = t;
        rec.p = intersect_point;
        rec.set_face_normal(r, unit_vector(normal)); // Might not be normalized
        rec.mat = mat;
        return true;
    }

    private:
    point3 vertex_a;
    point3 vertex_b;
    point3 vertex_c;
    vec3   normal;
    shared_ptr<material> mat;

};

#endif
