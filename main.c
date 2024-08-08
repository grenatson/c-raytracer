#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

#include <stdio.h>

void random_scene() {
    struct Material ground = material_make(LAMBERTIAN,
                                           color_make(0.5, 0.5, 0.3));
    world_add( hittable_make(SPHERE, sphere_make(point_make(0, -1000, 0),
                                                 1000,
                                                 ground))
    );

    for (int a = -21; a < 21; a++) {
        for (int b = -21; b < 21; b++) {
            double choose_mat = frand();
            point3 center = { a + 0.9*frand(), 0.2, b + 0.9*frand() };

            if (vec3_len(vec3_diff(center, point_make(4,0.2,0))) > 0.9) {
                struct Material sphere_mat;

                if (choose_mat < 0.5) { // lambertian
                    color albedo = vec3_mult_vec(vec3_random(),
                                                 vec3_random());
                    sphere_mat = material_make(LAMBERTIAN, albedo);
                } else if (choose_mat < 0.8) { // metal
                    color albedo = vec3_random_lim(0.5, 1);
                    double fuzz = frand_lim(0, 0.5);
                    sphere_mat = material_make(METAL, albedo, fuzz);
                } else { // glass
                    sphere_mat = material_make(DIELECTRIC, 1.5);
                }

                world_add( hittable_make(
                            SPHERE,
                            sphere_make(center, 0.2, sphere_mat))
                );
            }
        }
    }

    // add glass ball
    struct Material mat1 = material_make(DIELECTRIC, 1.5);
    point3 p1 = { 4, 1, -1 };
    world_add( hittable_make(SPHERE, sphere_make(p1, 1.0, mat1)) );
    world_add( hittable_make(SPHERE, sphere_make(p1, -0.7, mat1)) );

    // add metal sphere
    struct Material mat2 = material_make(METAL,
                                         color_make(0.4, 0.2, 0.1),
                                         0.2);
    point3 p2 = { -2, 1, 2 };
    world_add( hittable_make(SPHERE, sphere_make(p2, 1.0, mat2)) );

    // add metal sphere
    struct Material mat3 = material_make(METAL,
                                         color_make(0.7, 0.6, 0.5),
                                         0.0);
    point3 p3 = { 1, 1, 0 };
    world_add( hittable_make(SPHERE, sphere_make(p3, 1.0, mat3)) );
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = (int) (image_width / aspect_ratio);
    const int samples_per_pixel = 20;
    const int max_depth = 75;

    // World
    random_scene();

#if 0
    struct Material ground = material_make(LAMBERTIAN, 
                                           color_make(0.8, 0.8, 0.0));
    struct Material center = material_make(LAMBERTIAN, 
                                           color_make(0.1, 0.2, 0.3));
    struct Material left   = material_make(DIELECTRIC, 
                                           1.5);
    struct Material right  = material_make(METAL, 
                                           color_make(0.8, 0.6, 0.2),
                                           0.0);

    point3 p1 = { 0.0, -100.5, -1.0 };
    world_add( hittable_make(SPHERE, sphere_make(p1, 100, ground)) );

    point3 p2 = { 0.0, 0.0, -1.0 };
    world_add( hittable_make(SPHERE, sphere_make(p2, 0.5, ground)) );

    point3 p3 = { -1.2, 0.0, -1.2 };
    world_add( hittable_make(SPHERE, sphere_make(p3, 0.5, ground)) );
    //world_add( hittable_make(SPHERE, sphere_make(p3, -0.4, ground)) );

    point3 p4 = { 1.5, 0.0, -1.5 };
    world_add( hittable_make(SPHERE, sphere_make(p4, 0.5, ground)) );
#endif

    // Camera
    point3 lookfrom = { 13, 3, 2 };
    point3 lookat = { 0, 0, 0 };
    vec3 vup = { 0, 1, 0 };
    double dist_to_focus = 12;
    double aperture = 0.1;

    struct Camera cam = camera_make(
            lookfrom, lookat, vup, 20, aspect_ratio,
            aperture, dist_to_focus
    );

    // Render

    printf("P3\n %d %d\n255\n", image_width, image_height);

    for (int j = image_height-1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %3d", j);
        for (int i = 0; i < image_width; ++i) {
            color pixel_color = { 0, 0, 0 };
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + frand()) / (image_width-1);
                double v = (j + frand()) / (image_height-1);
                ray r = camera_get_ray(cam, u, v);
                pixel_color = vec3_add(2, pixel_color,
                                       ray_color(r, max_depth));
            }
            write_color(stdout, pixel_color, samples_per_pixel);
        }
    }

    fprintf(stderr, "\nAll %d lines are scanned\n", image_height);
    return 0;
}

