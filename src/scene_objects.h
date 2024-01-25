#ifndef SCENE_OBJECTS_H
#define SCENE_OBJECTS_H

#include "interval.h"
#include "ray.h"

class hit_record {
    public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record's normal vector
        // NOTE: the parameter `outward_normal` is assumed to have unit length
        // This let's geometry code normalize with potential optimizations

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class scene_object {
    // Abstract class which represents objects in the scene such as spheres, triangles, etc...
    public:
    virtual ~scene_object() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif
