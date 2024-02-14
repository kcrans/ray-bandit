#include "rtcommon.h"

#include "camera.h"
#include "color.h"
#include "scene_objects_list.h"
#include "material.h"
#include "sphere.h"

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

int main(int argc, char *argv[]) {
    
    std::string filename;
    std::cout << "Enter filename (without extension) to save render as:" << std::endl;
    std::getline(std::cin, filename);
    auto current_time = std::chrono::system_clock::now();
    auto current_time_formated = std::chrono::system_clock::to_time_t(current_time);

    if (filename.empty()) {
        // Use current data and time as a filename if none was given
        std::stringstream date_stream;
        date_stream << std::put_time(std::localtime(&current_time_formated), "%Y-%m-%d_%H-%M-%S");
        filename = date_stream.str();
    }

    scene_objects_list world;
    // Generate one spehere in the center of the screen
    //world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // Generate a spehere that acts as the 'land' in the image
    //world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Ground
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    
    world.add(make_shared<sphere>(point3( 0.0,    -0.3333, -1.0),   0.16, material_center));
    world.add(make_shared<sphere>(point3(-0.5,    -0.3333, -1.0),   0.16, material_right));
    world.add(make_shared<sphere>(point3(-1.0,    -0.3333, -1.0),   0.16, material_left));
    world.add(make_shared<sphere>(point3( 0.5,    -0.3333, -1.0),   0.16, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    -0.3333, -1.0),   0.16, material_right));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.16, material_center));
    world.add(make_shared<sphere>(point3(-0.5,    0.0, -1.0),   0.16, material_right));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.16, material_left));
    world.add(make_shared<sphere>(point3( 0.5,    0.0, -1.0), 0.16, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.16, material_right));
    world.add(make_shared<sphere>(point3( 0.0,    0.3333, -1.0),   0.16, material_center));
    world.add(make_shared<sphere>(point3(-0.5,    0.3333, -1.0), 0.16, material_right));
    world.add(make_shared<sphere>(point3(-1.0,    0.3333, -1.0),   0.16, material_left));
    world.add(make_shared<sphere>(point3( 0.5,    0.3333, -1.0), 0.16, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.3333, -1.0),   0.16, material_right));



    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 300;
    cam.sample_size  = 200; // Number of samples to take for each pixel 
    cam.max_depth    = 500;  // Max number of times a ray can reflect

    cam.render(world, filename);

    auto finished_time = std::chrono::system_clock::now();
    auto finished_time_formated = std::chrono::system_clock::to_time_t(finished_time);

    std::cout << "Render completed in " << std::difftime(finished_time_formated, current_time_formated) << " seconds." << std::endl; 
    return 0;
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
