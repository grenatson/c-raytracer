#ifndef WORLD_H
#define WORLD_H

#include "utility.h"
#include "vec3.h"

struct Material;
struct Hittable;

struct Hit_record {
    point3 p;
    vec3 normal;
    struct Material *mat_ptr;
    double t;
    bool front_face;
};

// Materials
enum material_type { LAMBERTIAN, METAL, DIELECTRIC };

struct Lambertian {
    color albedo;
};

struct Metal {
    color albedo;
    double fuzz;
};

struct Dielectric {
    double ir; // index of refraction
};

struct Material {
    enum material_type type;
    union {
        struct Lambertian as_lambertian;
        struct Metal as_metal;
        struct Dielectric as_dielectric;
    };

    bool (*scatter)(struct Material *mat_ptr, ray r_in,
                    struct Hit_record* rec, 
                    color *attenuation, ray *scattered);
};

struct Material material_make(enum material_type type, ...);

// Hittables
enum hittable_type { SPHERE };

typedef struct Sphere {
    point3 center;
    double radius;
    struct Material mat;
} sphere;

struct Hittable {
    enum hittable_type type;
    union {
        sphere as_sphere;
    };

    bool (*hit)(struct Hittable*, ray, 
                double, double, struct Hit_record*);
};

sphere sphere_make(point3 c, double r, struct Material m);
struct Hittable hittable_make(enum hittable_type type, ...);

// World
bool world_add(struct Hittable h);
bool world_del();
bool world_hit(ray r, double t_min, double t_max, struct Hit_record *rec);

#endif
