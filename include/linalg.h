#ifndef MINI3D_LINALG_H
#define MINI3D_LINALG_H

typedef struct { int x, y; } vec2i;

typedef struct { float x, y, z; } vec3f;
typedef struct { float m[9]; } matrix3;

vec3f make_vec3f(float x, float y, float z);

#endif