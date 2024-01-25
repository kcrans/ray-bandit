#ifndef CAMERA_H
#define CAMERA_H

#include "rtcommon.h"

#include "color.h"
#include "scene_objects.h"

#ifdef __clang__
#define STBIWDEF static inline
#endif
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <fstream>
//#include <cstdint>

class camera {
    public:
    double aspect_ratio = 1.0; // width over height
    int    image_width  = 100; // default width pixel count
    int    sample_size  = 10;  // number of random samples for each pixel
    int       max_depth = 10;  // max number of times a ray can bounce
    void render(const scene_object& world) {
        initialize();
        std::ofstream img_file;
        img_file.open("./images/test1.ppm");
        img_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        uint8_t* img_rgb = new uint8_t[image_width*image_height*3];

        int count = 0;
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rCurrent Scanline: " << j << '/' << image_height - 1 << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < sample_size; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                uint8_t rgb[3]; 
                write_color(img_file, pixel_color, sample_size, rgb);

                img_rgb[count++] = rgb[0];
                img_rgb[count++] = rgb[1];
                img_rgb[count++] = rgb[2];

            }
        }

        stbi_write_jpg("./images/test.jpg", image_width, image_height, 3, img_rgb, 100);
        delete[] img_rgb;
        img_file.close();
        std::clog << "\rDone.                    \n";
    }

    private:
    int image_height;     // rendered image height
    point3 camera_center; // camera center coordinates
    point3 pixel00_loc;   // location of first pixel (0, 0)
    vec3 pixel_delta_u;   // offset to pixels horizontally
    vec3 pixel_delta_v;	  // offset to pixels vertically

    void initialize() {
        // Calculate height from width and aspect ratio
        // height must be greater than 1 pixel
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height; 
        
        camera_center = point3(0, 0, 0);
        
	// Determine viewport dimensions:
        // Viewport heights and widths are real-valued
        // The aspect ratio is based off the calulated pixel aspect ratio
        auto focal_length = 1.0;
        auto viewport_height = 2.0; // Arbitrary value
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel
        auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const {
        // returns a random ray for the pixel at i, j
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample =  pixel_center + pixel_sample_square();

        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);

    }

    vec3 pixel_sample_square() const {
        
        auto rand_x = random_double(-0.5, 0.5);
        auto rand_y = random_double(-0.5, 0.5);

        return (rand_x * pixel_delta_u) + (rand_y * pixel_delta_v); 
    }

    color ray_color(const ray& r, int depth, const scene_object& world) const {
        
        // if we've exceeded the depth limit, no more light is propogated
        if (depth <= 0) 
            return color(0, 0, 0);
        
        hit_record rec;

        // if ray hits surface, calculate scattered rays
        // in order to account for round-off error, let t = 0.001
        // so the next ray's origin is not inside the object
        if (world.hit(r, interval(0.001, infinity), rec)) {
            vec3 direction = rec.normal + random_unit_vector();
            return 0.80 * ray_color(ray(rec.p, direction), depth - 1,  world);
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

};

#endif
