#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtcommon.h"

class hit_record;

class material {
	public:
	virtual ~material() = default;
    // Properties of a material:
    // ambient colors
    // diffuse colors
    // specular colors
    // texture
    // refractive index
	virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scatter) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            
            // Catch degenerate scatter directions which occur if the scatter direction vector
            // is really close to (0, 0, 0)
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
}; 

class metal : public material {
    // Class for metals, which are inherently relfective
    // The fuzz factor scales the random unit vector added to the end of the reflected ray.
    // I.e. a fuzz factor of 1 will add a whole unit vector to the reflected ray, causing
    // it to change direction signifcantly. If the new ray points inwards to the object,
    // we just say that the object absorbed the ray and return false. 
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray&scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_unit_vector());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};

#endif
