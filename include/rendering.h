#ifndef RENDERING_H
#define RENDERING_H

#include "utility.h"
#include "vec3.h"
#include "camera.h"

struct Render {
    double aspect_ratio;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;
};

struct Render render_init(double ar, int w, int spp, int maxd);

// write color to .ppm file 
bool write_color(FILE *fp, color pixel_color, int samples_per_pixel);

void render(struct Render ren, struct Camera cam);

#endif
