#ifndef MINI3D_LINALG_H
#define MINI3D_LINALG_H

typedef struct { int x, y; } vec2i;

typedef struct { float x, y, z; } vec3f;
typedef struct { float x, y, z, w; } vec4f;

typedef struct { float m[9]; } matrix3;
typedef struct { float m[16]; } matrix4f;

vec3f make_vec3f(float x, float y, float z);

void make_identity(matrix4f *matrix);
void make_translation(matrix4f *matrix, vec3f t);
float dot(vec4f a, vec4f b);
void multiply(matrix4f *a, matrix4f *b, matrix4f *out);
vec3f add(vec3f a, vec3f b);
vec4f transform_vector(matrix4f *m, vec4f v);

#endif