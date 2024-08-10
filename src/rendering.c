#include "rendering.h"

struct Render render_init(double ar, int w, int spp, int maxd) {
    struct Render rndr = { .aspect_ratio = ar,
                           .image_width = w,
                           .image_height = (int) (w / ar),
                           .samples_per_pixel = spp,
                           .max_depth = maxd
    };
    return rndr;
}

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

void render(struct Render rndr, struct Camera cam) {
    printf("P3\n %d %d\n255\n", rndr.image_width, rndr.image_height);

    for (int j = rndr.image_height-1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %3d", j);
        for (int i = 0; i < rndr.image_width; ++i) {
            color pixel_color = { 0, 0, 0 };
            for (int s = 0; s < rndr.samples_per_pixel; ++s) {
                double u = (i + frand()) / (rndr.image_width-1);
                double v = (j + frand()) / (rndr.image_height-1);
                ray r = camera_get_ray(cam, u, v);
                pixel_color = vec3_add(2, pixel_color,
                                       ray_color(r, rndr.max_depth));
            }
            write_color(stdout, pixel_color, rndr.samples_per_pixel);
        }
    }

    fprintf(stderr, "\nAll %d lines are scanned\n", rndr.image_height);
}
