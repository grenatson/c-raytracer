#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

// Constants
const double PI = 3.14159265358979;

// Utility Macros & Functions
#define deg2rad(deg) ((double) (deg) * PI / 180.0)

double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Random Macros
#define frand() ((double) rand() / (RAND_MAX+1.0))
#define frand_lim(min, max) ((min) + ((max) - (min)) * frand())

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif
