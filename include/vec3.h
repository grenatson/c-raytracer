#ifndef VEC3_H
#define VEC3_H

#include "utility.h"

struct Vec3 {
    double e[3];
};

typedef struct Vec3 vec3;
typedef struct Vec3 point3;
typedef struct Vec3 color;

double vec3_x(vec3 a);
double vec3_y(vec3 a);
double vec3_z(vec3 a);

vec3 vec3_make(double x, double y, double z);
color color_make(double r, double g, double b);
point3 point_make(double x, double y, double z);

// return -a
vec3 vec3_neg(vec3 a);

#if 0
vec3 vec3_add(vec3 a, vec3 b);
#endif

// add n vectors
vec3 vec3_add(int n, vec3 a, ...);
vec3 vec3_diff(vec3 a, vec3 b);
vec3 vec3_mult(vec3 a, double t);
vec3 vec3_mult_vec(vec3 a, vec3 b);
vec3 vec3_div(vec3 a, double t);

double vec3_len_sq(vec3 a);
double vec3_len(vec3 a);
double vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);

vec3 vec3_unit(vec3 a);

// return true if all coordinates are less than 1e-8 in abs value
bool vec3_near_zero(vec3 a);

// reflect vector from surface with normal
vec3 vec3_reflect(vec3 a, vec3 n);

// refract vec from surface with normal and relative reflective index
vec3 vec3_refract(vec3 a, vec3 n, double etai_over_etat);

// vec3 random functions

// all coordinates are random
vec3 vec3_random();
vec3 vec3_random_lim(double min, double max);

// random in unit sphere
vec3 vec3_random_in_sphere();
vec3 vec3_random_unit();

// random in unit hemisphere
vec3 vec3_random_in_hemisphere(vec3 normal);

// random in unit disk
vec3 vec3_random_in_disk();

// RAY
typedef struct Ray {
    point3 orig;
    vec3 dir;
} ray;

ray ray_make(point3 orig, vec3 dir);
point3 ray_at(ray r, double t);

#endif

