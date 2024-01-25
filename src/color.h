#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <cstdint>
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    // Encode our color in a gamma-compressed
    // gamma = 2.2 in this case
    return std::pow(linear_component, 1.0 / 2.2);
    // Alternative, faster operation at the expense of accuracy
    // return sqrt(linear_component)
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel, uint8_t* rgb) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // divide color by number of samples
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // apply the linear to gamma transform
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // write the translated [0, 255] value of each color component
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';

    rgb[0] = static_cast<uint8_t>(256 * intensity.clamp(r));
    rgb[1] = static_cast<uint8_t>(256 * intensity.clamp(g));
    rgb[2] = static_cast<uint8_t>(256 * intensity.clamp(b));

}

#endif
