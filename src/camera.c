#include "camera.h"

struct Camera camera_make(
        point3 lookfrom,
        point3 lookat, 
        vec3   vup,
        double vfov,
        double aspect_ratio,
        double aperture,
        double focus_dist
) {
    struct Camera cam;

    double theta = deg2rad(vfov);
    double h = tan(theta/2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    cam.w = vec3_unit(vec3_diff(lookfrom, lookat));
    cam.u = vec3_unit(vec3_cross(vup, cam.w));
    cam.v = vec3_cross(cam.w, cam.u);

    double focal_length = 1.0;

    cam.origin = lookfrom;
    cam.horizontal = vec3_mult(cam.u, viewport_width * focus_dist);
    cam.vertical = vec3_mult(cam.v, viewport_height * focus_dist);

    vec3 proj_plane_center = vec3_diff(cam.origin,
                                       vec3_mult(cam.w, focus_dist));
    vec3 diagonal = vec3_div(vec3_add(2, cam.horizontal, cam.vertical),
                             2);
    cam.lower_left_corner = vec3_diff(proj_plane_center, diagonal);

    cam.lens_radius = aperture / 2;

    return cam;
}

ray camera_get_ray(struct Camera cam, double s, double t) {
    vec3 rd = vec3_mult(vec3_random_in_disk(), cam.lens_radius);
    vec3 offset = vec3_add(2, vec3_mult(cam.u, vec3_x(rd)),
                           vec3_mult(cam.v, vec3_y(rd)));

    vec3 orig2corner = vec3_diff(cam.lower_left_corner, cam.origin);
    orig2corner = vec3_diff(orig2corner, offset);
    
    vec3 point_on_screen = vec3_add(2, vec3_mult(cam.horizontal, s),
                                    vec3_mult(cam.vertical, t));
    return ray_make(vec3_add(2, cam.origin, offset),
                    vec3_add(2, orig2corner, point_on_screen));
}

color ray_color(ray r, int depth) {
    struct Hit_record rec;

    // If we've exceeded bounce limit, no light is gathered
    if (depth <= 0)
        return (color) { 0, 0, 0 };

    if (world_hit(r, 0.001, INFINITY, &rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(rec.mat_ptr, r, &rec, 
                                 &attenuation, &scattered))
            return vec3_mult_vec(ray_color(scattered, depth-1),
                                 attenuation);
        return (color) { 0, 0, 0 };
    }
    
    vec3 unit_direction = vec3_unit(r.dir);
    double t = 0.5 * (vec3_y(unit_direction) + 1.0);
    return vec3_add(2, vec3_mult(vec3_make(1.0, 1.0, 1.0), 1.0-t),
                       vec3_mult(vec3_make(0.5, 0.7, 1.0), t));
}

