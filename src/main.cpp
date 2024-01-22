#include "rtcommon.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

#include <string>

int main(int argc, char *argv[]) {
    hittable_list world;
    // Generate one spehere in the center of the screen
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // Generate a spehere that acts as the 'land' in the image
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 800;
    cam.sample_size  = 100; // Number of samples to take for each pixel 
    cam.max_depth    = 50;  // Max number of times a ray can reflect

    cam.render(world);
/*
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
    } */
}
