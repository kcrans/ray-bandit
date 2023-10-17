#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2*dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius*radius;
	auto discriminant = b*b - 4*a*c;
	return (discriminant >= 0);
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -20), 0.5, r))
	    return color(1, 0, 0);
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


int main() {

    // Image Specifications
    auto aspect_ratio = 16.0 / 9.0; // Ideal ratio
    int image_width = 400;

    // Calculate height from width and aspect ratio
    // height must be greater than 1 pixel
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; 

    // Camera

    auto focal_length = 1.0;
    // Viewport heights and widths are real-valued
    // The aspect ratio is based off the calulated pixel aspect ratio
    auto viewport_height = 2.0; // Arbitrary value
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    point3 camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Rendering Code

    std::ofstream img_file;
    img_file.open("test.ppm");
    img_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rCurrent Scanline: " << j << '/' << image_height - 1 << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(img_file, pixel_color);
        }
    }
    img_file.close();
    std::clog << "\rDone.                    \n";
}
