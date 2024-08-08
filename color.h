#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <stdio.h>

bool write_color(FILE *fp, color pixel_color, int samples_per_pixel) {
    double r = vec3_x(pixel_color);
    double g = vec3_y(pixel_color);
    double b = vec3_z(pixel_color);

    // normalise for samples gamma-correction for gamma=2.0
    r = sqrt(r / samples_per_pixel);
    g = sqrt(g / samples_per_pixel);
    b = sqrt(b / samples_per_pixel);

    return fprintf(fp, "%d %d %d\n",
                   (int) (256 * clamp(r, 0.0, 0.999)),
                   (int) (256 * clamp(g, 0.0, 0.999)),
                   (int) (256 * clamp(b, 0.0, 0.999))
           );
}

#endif
