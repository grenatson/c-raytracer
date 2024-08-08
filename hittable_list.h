#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#define MAXHITTABLE 2000

struct hittable_list {
   struct hittable array[MAXHITTABLE];
   int count;
} WORLD = { .count = 0 };

bool world_add(struct hittable h) {
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

bool world_hit(ray r, double t_min, double t_max, struct hit_record *rec) {
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (int i = 0; i < WORLD.count; ++i) {
        struct hittable *h_ptr = WORLD.array + i;
        if (h_ptr->hit(h_ptr, r, t_min, closest_so_far, rec)) {
            hit_anything = true;
            closest_so_far = rec->t;
        }
    }

    return hit_anything;
}

#endif
