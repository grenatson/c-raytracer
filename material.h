#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

struct Lambertian {
    color albedo;
};

struct Metal {
    color albedo;
    double fuzz;
};

struct Dielectric {
    double ir; // Index of refraction
};

enum material_type { LAMBERTIAN, METAL, DIELECTRIC };

struct Material {
    enum material_type type;
    union {
        struct Lambertian as_lambertian;
        struct Metal as_metal;
        struct Dielectric as_dielectric;
    };

    bool (*scatter)(struct Material *mat_ptr, ray r_in,
                    struct hit_record* rec, 
                    color *attenuation, ray *scattered);
};

bool lambertian_scatter(struct Material *mat_ptr, ray r_in,
                        struct hit_record* rec, 
                        color *attenuation, ray *scattered) {
    vec3 scatter_direction = vec3_add(2, rec->normal, vec3_random_unit());

    // catch degenerate scatter direction
    if (vec3_near_zero(scatter_direction))
        scatter_direction = rec->normal;

    *scattered = ray_make(rec->p, scatter_direction);
    *attenuation = mat_ptr->as_lambertian.albedo;
    return true;
}

bool metal_scatter(struct Material *mat_ptr, ray r_in,
                   struct hit_record* rec,
                   color *attenuation, ray *scattered) {
    vec3 reflected = vec3_reflect(vec3_unit(r_in.dir), rec->normal);

    double fuzz = mat_ptr->as_metal.fuzz;
    vec3 direction = vec3_add(2, reflected,
                              vec3_mult(vec3_random_in_sphere(), fuzz));
    
    *scattered = ray_make(rec->p, direction);
    *attenuation = mat_ptr->as_metal.albedo;
    return (vec3_dot(reflected, rec->normal) > 0);
}

double dielectric_reflectance(double cosine, double ref_idx) {
    // Schlick's approximation
    double r0 = (1-ref_idx) / (1+ref_idx);
    r0 *= r0;
    return r0 + (1-r0) * pow( (1-cosine), 5 );
}

bool dielectric_scatter(struct Material *mat_ptr, ray r_in,
                        struct hit_record* rec,
                        color *attenuation, ray *scattered) {
    *attenuation = color_make( 1, 1, 1 );
    double ir = mat_ptr->as_dielectric.ir;
    double refraction_ratio = rec->front_face ? (1.0/ir) : ir;

    vec3 unit_dir = vec3_unit(r_in.dir);
    double cos_theta = fmin(vec3_dot(vec3_neg(unit_dir), rec->normal), 1);
    double sin_theta = sqrt(1 - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    vec3 direction;
    if (cannot_refract 
        || dielectric_reflectance(cos_theta, refraction_ratio) > frand())
        direction = vec3_reflect(unit_dir, rec->normal);
    else
        direction = vec3_refract(unit_dir, rec->normal, refraction_ratio);

    *scattered = ray_make(rec->p, direction);
    return true;
}

struct Material material_make(enum material_type type, ...) {
    va_list init;
    va_start(init, type);

    struct Material res;
    res.type = type;

    switch (type) {
        case LAMBERTIAN:
            res.as_lambertian.albedo = va_arg(init, color);
            res.scatter = &lambertian_scatter;
            break;
        case METAL:
            res.as_metal.albedo = va_arg(init, color);
            res.as_metal.fuzz = va_arg(init, double);
            res.scatter = &metal_scatter;
            break;
        case DIELECTRIC:
            res.as_dielectric.ir = va_arg(init, double);
            res.scatter = &dielectric_scatter;
            break;
        default:
            fprintf(stderr, "Material isn't found!");
            exit(1);
    }

    va_end(init);
    return res;
}

#endif
