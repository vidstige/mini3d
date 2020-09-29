#ifndef MINI3D_LINALG_H
#define MINI3D_LINALG_H

typedef struct { int x, y; } vec2i;

typedef struct { float x, y, z; } vec3f;
typedef struct { float x, y, z, w; } vec4f;

typedef struct { float m[9]; } matrix3;
typedef struct { float m[16]; } matrix4f;

vec3f make_vec3f(float x, float y, float z);

void identity(matrix3 *matrix);
vec3f row(matrix3* matrix, int i);
vec3f column(matrix3* matrix, int i);
float dot(vec3f a, vec3f b);
void multiply(matrix3 *a, matrix3 *b, matrix3 *out);
vec3f add(vec3f a, vec3f b);
vec3f transform(matrix3 *m, vec3f t, vec3f v);

#endif