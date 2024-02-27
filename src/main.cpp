#include "common.h"

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

    auto ground_material = make_shared<lambertian1>(color(0.5, 0.5, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian1>(albedo, 0.0);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian1>(color(0.4, 0.2, 0.1), 0.0);
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 1200;
    cam.sample_size  = 500; // Number of samples to take for each pixel 
    cam.max_depth    = 50;  // Max number of times a ray can reflect
                            
    cam.v_fov     = 20;
    cam.look_from = point3(13, 2, 3);
    cam.look_at    = point3(0, 0, 0);
    cam.v_up       = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

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
