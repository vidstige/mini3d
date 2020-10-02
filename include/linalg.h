#ifndef MINI3D_LINALG_H
#define MINI3D_LINALG_H

#include <stdbool.h>

typedef struct { int x, y; } vec2i;

typedef struct { float x, y, z; } vec3f;
typedef struct { float x, y, z, w; } vec4f;

typedef struct { float m[9]; } matrix3;
typedef struct { float m[16]; } matrix4f;

vec3f make_vec3f(float x, float y, float z);
vec3f vec3f_subtract(vec3f a, vec3f b);
float vec3f_dot(vec3f a, vec3f b);
vec3f cross(vec3f a, vec3f b);
float dot3f(vec3f a, vec3f b);
float norm3f(vec3f v);
vec3f normalized3f(vec3f v);

vec4f make_vec4f(float x, float y, float z, float w);

void make_identity(matrix4f *matrix);
void make_translation(matrix4f *matrix, vec3f t);
float dot(vec4f a, vec4f b);
void multiply(matrix4f *a, matrix4f *b, matrix4f *out);
vec3f add(vec3f a, vec3f b);
vec4f transform_vector(matrix4f *m, vec4f v);

matrix4f transpose4f(matrix4f *matrix);
bool invert4f(matrix4f *matrix, matrix4f *out);

#endif