#include "color.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

int main() {

    // Image Specifications

    int image_width = 256;
    int image_height = 256;

    // Rendering Code

    std::ofstream img_file;
    img_file.open("test.ppm");
    img_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rCurrent Scanline: " << j << '/' << image_height - 1 << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            write_color(img_file, pixel_color);
        }
    }
    img_file.close();
    std::clog << "\rDone.                    \n";
}