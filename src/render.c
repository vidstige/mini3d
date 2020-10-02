#include <stdbool.h>
#include <math.h>

#include "render.h"

#define EPSILON 0.000001f

typedef struct {
    vec3f min, max;
} bbox3f;

void init_bbox(bbox3f *bbox) {
    bbox->min = make_vec3f(INFINITY, INFINITY, INFINITY);
    bbox->max = make_vec3f(-INFINITY, -INFINITY, -INFINITY);
}
void update_bbox(bbox3f *bbox, vec3f v) {
    bbox->min.x = fmin(bbox->min.x, v.x); bbox->min.y = fmin(bbox->min.y, v.y); bbox->min.z = fmin(bbox->min.z, v.z);
    bbox->max.x = fmax(bbox->max.x, v.x); bbox->max.y = fmax(bbox->max.y, v.y); bbox->max.z = fmax(bbox->max.z, v.z);
}

void pixel(uint32_t *buffer, float *z_buffer, int x, int y, float z, uint32_t color) {
    if (x >= 0 && x < 320 && y >= 0 && y < 200) {
        if (z_buffer[x + y * 320] > z) {
            buffer[x + y * 320] = color;
            z_buffer[x + y * 320] = z;
        }
    }
}

typedef struct { float alpha, beta, gamma; } barycentric_t;

float edge_function(vec3f v0, vec3f v1, vec3f v2) {
    return (v2.x - v0.x) * (v1.y - v0.y) - (v2.y - v0.y) * (v1.x - v0.x);
}

barycentric_t as_barycentric(vec3f v0, vec3f v1, vec3f v2, vec3f p) {
    barycentric_t out;
    float area = edge_function(v0, v1, v2);
    if (area < EPSILON) {
        return out;
    }    
    out.alpha = edge_function(v1, v2, p) / area;
    out.beta = edge_function(v2, v0, p) / area;
    out.gamma = edge_function(v0, v1, p) / area;
    return out;
}

bool contains_point(barycentric_t barycentric) {
    return barycentric.alpha >= 0 && barycentric.beta >= 0 && barycentric.gamma >= 0; 
}

float interpolate_barycentric(float a, float b, float c, barycentric_t barycentric) {
    return a * barycentric.alpha + b * barycentric.beta + c * barycentric.gamma;
}

void render_triangle(uint32_t *buffer, float *z_buffer, uint32_t color, vec3f v0, vec3f v1, vec3f v2) {
    bbox3f bbox;
    int x, y;
    float z;
    barycentric_t barycentric;
    init_bbox(&bbox);
    update_bbox(&bbox, v0);
    update_bbox(&bbox, v1);
    update_bbox(&bbox, v2);
    for (y = (int)bbox.min.y; y < (int)bbox.max.y; y++) {
        for (x = (int)bbox.min.x; x < (int)bbox.max.x; x++) {
            barycentric = as_barycentric(v0, v1, v2, make_vec3f(x, y, 1));
            if (contains_point(barycentric)) {
                z = interpolate_barycentric(v0.z, v1.z, v2.z, barycentric);
                pixel(buffer, z_buffer, x, y, z, color);
            }
        }
    }
}