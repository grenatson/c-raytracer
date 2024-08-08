#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include <stdio.h>

struct Vec3 {
    double e[3];
};

typedef struct Vec3 vec3;
typedef struct Vec3 point3;
typedef struct Vec3 color;

double vec3_x(vec3 a) { return a.e[0]; }
double vec3_y(vec3 a) { return a.e[1]; }
double vec3_z(vec3 a) { return a.e[2]; }

vec3 vec3_make(double x, double y, double z) {
    vec3 a;
    a.e[0] = x; a.e[1] = y; a.e[2] = z;
    return a;
}

color color_make(double r, double g, double b) {
    return vec3_make(r, g, b);
}

point3 point_make(double x, double y, double z) {
    return vec3_make(x, y, z);
}

vec3 vec3_neg(vec3 a) {
    a.e[0] *= -1; a.e[1] *= -1; a.e[2] *= -1;
    return a;
}

#if 0
vec3 vec3_add(vec3 a, vec3 b) {
    a.e[0] += b.e[0];
    a.e[1] += b.e[1];
    a.e[2] += b.e[2];
    return a;
}

vec3 vec3_addn(int n, vec3 a, ...);
#endif

vec3 vec3_add(int n, vec3 a, ...) {
    vec3 temp; // for storing vectors from variable list
    va_list vp; // points to each vec3
    va_start(vp, a);

    while (--n > 0) {
        temp = va_arg(vp, vec3);
        a.e[0] += temp.e[0];
        a.e[1] += temp.e[1];
        a.e[2] += temp.e[2];
    }

    va_end(vp);
    return a;
}

vec3 vec3_diff(vec3 a, vec3 b) {
    a.e[0] -= b.e[0];
    a.e[1] -= b.e[1];
    a.e[2] -= b.e[2];
    return a;
}

vec3 vec3_mult(vec3 a, double t) {
    a.e[0] *= t; a.e[1] *= t; a.e[2] *= t;
    return a;
}

vec3 vec3_mult_vec(vec3 a, vec3 b) {
    a.e[0] *= b.e[0];
    a.e[1] *= b.e[1];
    a.e[2] *= b.e[2];
    return a;
}

vec3 vec3_div(vec3 a, double t) {
    a.e[0] *= 1/t; a.e[1] *= 1/t; a.e[2] *= 1/t;
    return a;
}

double vec3_len_sq(vec3 a) {
    return a.e[0] * a.e[0] + a.e[1] * a.e[1] + a.e[2] * a.e[2];
}

double vec3_len(vec3 a) {
    return sqrt(vec3_len_sq(a));
}

double vec3_dot(vec3 a, vec3 b) {
    return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_make(a.e[1] * b.e[2] - a.e[2] * b.e[1],
                     a.e[2] * b.e[0] - a.e[0] * b.e[2],
                     a.e[0] * b.e[1] - a.e[1] * b.e[0]);
}

vec3 vec3_unit(vec3 a) {
    return vec3_div(a, vec3_len(a));
}

bool vec3_near_zero(vec3 a) {
    double s = 1e-8;
    return (fabs(a.e[0]) < s) && (fabs(a.e[1]) < s) && (fabs(a.e[2]) < s);
}

vec3 vec3_reflect(vec3 a, vec3 n) {
    return vec3_diff(a, vec3_mult(n, 2*vec3_dot(a, n)));
}

vec3 vec3_refract(vec3 a, vec3 n, double etai_over_etat) {
    double cos_theta = fmin(vec3_dot(vec3_neg(a), n), 1.0);
    vec3 r_out_perp = vec3_add(2, a, vec3_mult(n, cos_theta));
    r_out_perp = vec3_mult(r_out_perp, etai_over_etat);

    double par_mult = -sqrt(fabs( 1 - vec3_len_sq(r_out_perp) ));
    vec3 r_out_parallel = vec3_mult(n, par_mult);

    return vec3_add(2, r_out_perp, r_out_parallel);
}

int vec3_print(FILE *fp, vec3 a) {
    return fprintf(fp, "%f %f %f", a.e[0], a.e[1], a.e[2]);
}

// vec3 random functions
vec3 vec3_random() {
    return vec3_make(frand(), frand(), frand());
}

vec3 vec3_random_lim(double min, double max) {
    return vec3_make(frand_lim(min,max),
                     frand_lim(min,max),
                     frand_lim(min,max));
}

vec3 vec3_random_in_sphere() {
    while (true) {
        point3 p = vec3_random_lim(-1, 1);
        if (vec3_len_sq(p) >= 1) continue;
        return p;
    }
}

vec3 vec3_random_unit() {
    return vec3_unit(vec3_random_in_sphere());
}

vec3 vec3_random_in_hemisphere(vec3 normal) {
    vec3 in_unit_sphere = vec3_random_in_sphere();
    if (vec3_dot(in_unit_sphere, normal) > 0)
        return in_unit_sphere;
    else
        return vec3_neg(in_unit_sphere);
}

vec3 vec3_random_in_disk() {
    while (true) {
        vec3 p = vec3_make(frand_lim(-1, 1), frand_lim(-1, 1), 0);
        if (vec3_len_sq(p) >= 1) continue;
        return p;
    }
}

#endif
