#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct Material;

struct hit_record {
    point3 p;
    vec3 normal;
    struct Material *mat_ptr;
    double t;
    bool front_face;
};

#include "material.h"

struct Sphere {
    point3 center;
    double radius;
    struct Material mat;
};

typedef struct Sphere sphere;

enum hittable_type { SPHERE };

struct hittable {
    enum hittable_type type;
    union {
        sphere as_sphere;
    };

    bool (*hit)(struct hittable*, ray, 
                double, double, struct hit_record*);
};

void set_face_normal(ray r, vec3 outward_normal, struct hit_record *rec) {
    rec->front_face = (vec3_dot(r.dir, outward_normal) < 0);
    rec->normal = rec->front_face ? outward_normal 
                                  : vec3_neg(outward_normal);
}

sphere sphere_make(point3 c, double r, struct Material m) {
    sphere s;
    s.center = c; s.radius = r; s.mat= m;
    return s;
}

bool sphere_hit(struct hittable *h_ptr, ray r,
                double t_min, double t_max, struct hit_record *rec) {
    point3 center = h_ptr->as_sphere.center;
    double radius = h_ptr->as_sphere.radius;
    struct Material *mat_ptr = &(h_ptr->as_sphere.mat);

    // solve t^2 * (b,b) + 2t * (b, A-C) + (A-C, A-C) - r^2 = 0
    vec3 oc = vec3_diff(r.orig, center);
    double a = vec3_len_sq(r.dir);
    double half_b = vec3_dot(oc, r.dir);
    double c = vec3_len_sq(oc) - radius*radius;

    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    double sqrtd = sqrt(discriminant);

    // find the nearest root from the range [t_min, t_max]
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec->t = root;
    rec->p = ray_at(r, root);
    vec3 outward_normal = vec3_div(vec3_diff(rec->p, center), radius);
    set_face_normal(r, outward_normal, rec);
    rec->mat_ptr = mat_ptr;

    return true;
}

struct hittable hittable_make(enum hittable_type type, ...) {
    va_list init;
    va_start(init, type);

    struct hittable res;

    switch (type) {
        case SPHERE:
            res.type = SPHERE;
            res.as_sphere = va_arg(init, sphere);
            res.hit = &sphere_hit;
            break;
        default:
            fprintf(stderr, "Hittable type isn't found!");
            exit(1);
    }

    va_end(init);
    return res;
}

#endif
