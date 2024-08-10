#include "utility.h"
#include "vec3.h"
#include "camera.h"
#include "world.h"
#include "rendering.h"

void random_scene() {
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

                world_add(hittable_make(SPHERE, center, 0.2, sphere_mat));
            }
        }
    }

    // add glass ball
    struct Material mat1 = material_make(DIELECTRIC, 1.5);
    point3 p1 = { 4, 1, -1 };
    world_add( hittable_make(SPHERE, p1, 1.0, mat1) );
    world_add( hittable_make(SPHERE, p1, -0.7, mat1) );

    // add metal sphere
    struct Material mat2 = material_make(METAL,
                                         color_make(0.9, 0.1, 0.1),
                                         0.2);
    point3 p2 = { -2, 1, 2 };
    world_add( hittable_make(SPHERE, p2, 1.0, mat2) );

    // add metal sphere
    struct Material mat3 = material_make(METAL,
                                         color_make(0.7, 0.6, 0.5),
                                         0.0);
    point3 p3 = { 1, 1, 0 };
    world_add( hittable_make(SPHERE, p3, 1.0, mat3) );

    // add ground
    struct Material mat4 = material_make(LAMBERTIAN,
                                         color_make(0.3, 0.6, 0.3));
    point3 p4 = { 0, -1000, 0 };
    world_add( hittable_make(SPHERE, p4, 1000.0, mat4) );

}

int main() {

    // Image and Render
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 100;
    int samples_per_pixel = 5;
    int max_depth = 10;

    const struct Render RENDER = render_init(
            aspect_ratio, image_width,
            samples_per_pixel, max_depth
    );

    // World scene
    random_scene();

    // Camera
    point3 lookfrom = { 13, 3, 2 };
    point3 lookat = { 0, 0, 0 };
    vec3 vup = { 0, 1, 0 };
    double dist_to_focus = 12;
    double aperture = 0.1;

    const struct Camera CAMERA = camera_make(
            lookfrom, lookat, vup, 20, aspect_ratio,
            aperture, dist_to_focus
    );

    // Rendering
    render(RENDER, CAMERA);

    return 0;
}

