#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

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

// Lambertian material which always scatters. The scattered rays are attenuated by the reflectance R.
class lambertian1: public material {
    public:
        lambertian1(const color& a, const double R) : albedo(a * (1 - R)) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            
            // Catch degenerate scatter directions which occur if the scatter direction vector
            // is really close to (0, 0, 0) and scatter exactly in the normal's direction
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

// Lambertian material which sometimes scatters (with probability 1 - R). The rays are not attenuated
class lambertian2 : public material {
    public:
        lambertian2(const color& a, const double R) : albedo(a), reflectance(R) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            // reflectance = prob(ray is reflected), so the probability a ray is scattered
            // is 1 - reflectance. This means when sampling from [0, 1) we scatter the ray
            // if the random double is < 1 - reflectance or equivalently > reflectance
            if (random_double() < reflectance) {
                return false;
            }
            auto scatter_direction = rec.normal + random_unit_vector();
            
            // Catch degenerate scatter directions which occur if the scatter direction vector
            // is really close to (0, 0, 0) and scatter exactly in the normal's direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
        double reflectance;
}; 

// Mixture of lambertian1 and lambertian2. The incident rays are scattered with probability p and
// have an attenuation of albedo / p
class lambertian3 : public material {
    public:
        lambertian3(const color& a, const double R) : albedo(a / (1 - R)), reflectance(R) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            
            if (random_double() < reflectance) {
                return false;
            }
            auto scatter_direction = rec.normal + random_unit_vector();
            
            // Catch degenerate scatter directions which occur if the scatter direction vector
            // is really close to (0, 0, 0) and scatter exactly in the normal's direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
        double reflectance;
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

class dielectric : public material {
    public:
        dielectric(double refractive_index_ratio) : rir(refractive_index_ratio) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            // 
            double refraction_ratio = rec.ray_facing_inwards ? (1.0/rir) : rir;

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, refracted);
            return true;
        }
    private:
        double rir; // Ratio of refractive indices n1 / n2 where n1 is the index for air and n2 the index
                    // for this particular dielectric material.
};

#endif
