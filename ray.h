#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray {
    point3 orig;
    vec3 dir;
};

typedef struct Ray ray;

ray ray_make(point3 orig, vec3 dir) {
    ray r;
    r.orig = orig; r.dir = dir;
    return r;
}

point3 ray_at(ray r, double t) {
    return vec3_add(2, r.orig, vec3_mult(r.dir, t));
}

#endif
