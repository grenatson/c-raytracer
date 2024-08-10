#include "../include/world.h"

// HITTABLES

void set_face_normal(ray r, vec3 outward_normal, struct Hit_record *rec) {
    rec->front_face = (vec3_dot(r.dir, outward_normal) < 0);
    rec->normal = rec->front_face ? outward_normal 
                                  : vec3_neg(outward_normal);
}

// Sphere functions
sphere sphere_make(point3 c, double r, struct Material m) {
    sphere s;
    s.center = c; s.radius = r; s.mat= m;
    return s;
}

bool sphere_hit(struct Hittable *h_ptr, ray r,
                double t_min, double t_max, struct Hit_record *rec) {
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

struct Hittable hittable_make(enum hittable_type type, ...) {
    va_list init;
    va_start(init, type);

    struct Hittable res;
    res.type = type;

    switch (type) {
        case SPHERE:
            point3 c = va_arg(init, point3);
            double r = va_arg(init, double);
            struct Material m = va_arg(init, struct Material);

            res.as_sphere = sphere_make(c, r, m);
            res.hit = &sphere_hit;
            break;
        default:
            fprintf(stderr, "Hittable type isn't found!");
            va_end(init);
            exit(1);
    }

    va_end(init);
    return res;
}

// MATERIALS

bool lambertian_scatter(struct Material *mat_ptr, ray r_in,
                        struct Hit_record* rec, 
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
                   struct Hit_record* rec,
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
                        struct Hit_record* rec,
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


// WORLD

struct {
   struct Hittable array[MAXHITTABLE];
   int count;
} WORLD = { .count = 0 };

bool world_add(struct Hittable h) {
    if (WORLD.count < MAXHITTABLE) {
        WORLD.array[WORLD.count++] = h;
        return true;
    }
    return false;
}

bool world_del() {
    if (WORLD.count > 0) {
        WORLD.count--;
        return true;
    }
    return false;
}

bool world_hit(ray r, double t_min, double t_max, struct Hit_record *rec) {
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (int i = 0; i < WORLD.count; ++i) {
        struct Hittable *h_ptr = WORLD.array + i;
        if (h_ptr->hit(h_ptr, r, t_min, closest_so_far, rec)) {
            hit_anything = true;
            closest_so_far = rec->t;
        }
    }

    return hit_anything;
}

