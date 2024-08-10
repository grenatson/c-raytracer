#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"
#include "world.h"

struct Camera {
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    vec3 lower_left_corner;
    vec3 u, v, w;
    double lens_radius;
};

struct Camera camera_make(
        point3 lookfrom,
        point3 lookat, 
        vec3   vup,
        double vfov,
        double aspect_ratio,
        double aperture,
        double focus_dist
); 

// get ray for coordinates s, t on the camera's plane
ray camera_get_ray(struct Camera cam, double s, double t);

// get color for the ray
color ray_color(ray r, int depth);

#endif

