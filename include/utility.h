#ifndef UTILITY_H
#define UTILITY_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

// Constants
#define MAXHITTABLE 2000
#define PI 3.14159265358979

// Utility Macros & Functions
#define deg2rad(deg) ((double) (deg) * PI / 180.0)

double clamp(double x, double min, double max);

// Random Macros
#define frand() ((double) rand() / (RAND_MAX+1.0))
#define frand_lim(min, max) ((min) + ((max) - (min)) * frand())

#endif

