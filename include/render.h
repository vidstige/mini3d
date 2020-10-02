#ifndef MINI3D_RENDER_H
#define MINI3D_RENDER_H

#include <stdint.h>

#include "linalg.h"

void render_triangle(uint32_t *buffer, float *z_buffer, uint32_t color, vec3f v0, vec3f v1, vec3f v2);

#endif