#include "rtcommon.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <fstream>
#include <string>


color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


int main(int argc, char *argv[]) {

    // Image Specifications
    auto aspect_ratio = 16.0 / 9.0; // Ideal ratio
    int image_width = 400;

    // Calculate height from width and aspect ratio
    // height must be greater than 1 pixel
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; 

    // World

    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    auto focal_length = 1;

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

    // Debug info
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "--info") {
            std::cout << "Ideal aspect ratio: " << aspect_ratio << std::endl;
            std::cout << "Pixel height and width: " << image_height << 'x' << image_width << std::endl;
            std::cout << "Viewport height and width: " << viewport_height << 'x' << viewport_width << std::endl;
            std::cout << "Camera center and focal length: " << camera_center << ',' << ' ' << focal_length << std::endl;
            std::cout << "V_{u}: " << viewport_u << std::endl;
            std::cout << "V_{v}: " << viewport_v << std::endl;
            std::cout << "Delta u: " << pixel_delta_u << std::endl;
            std::cout << "Delta v: " << pixel_delta_v << std::endl;
            std::cout << "Upper left viewport: " << viewport_upper_left << std::endl;
            std::cout << "1st pixel location: " << pixel00_loc << std::endl;
        }
    }

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

            color pixel_color = ray_color(r, world);
            write_color(img_file, pixel_color);
        }
    }
    img_file.close();
    std::clog << "\rDone.                    \n";
}
