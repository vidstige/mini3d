#ifndef MINI3D_RENDER_H
#define MINI3D_RENDER_H

#include <stdint.h>

#include "linalg.h"

void pixel(uint32_t *buffer, int x, int y, uint32_t color);
void render_triangle(uint32_t *buffer, float* z_buffer, vec3f a, vec3f b, vec3f c);

#endif