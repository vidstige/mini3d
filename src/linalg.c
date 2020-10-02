#include "linalg.h"
#include <math.h>

vec3f make_vec3f(float x, float y, float z) {
    vec3f out;
    out.x = x; out.y = y; out.z = z;
    return out;
}
vec3f vec3f_subtract(vec3f a, vec3f b) {
    return make_vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}
float vec3f_dot(vec3f a, vec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
vec3f cross(vec3f a, vec3f b) {
    return make_vec3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}
float dot3f(vec3f a, vec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float norm3f(vec3f v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3f normalized3f(vec3f v) {
    float l = norm3f(v);
    return make_vec3f(v.x / l, v.y / l, v.z / l);
}

void make_identity(matrix4f *matrix) {
    matrix->m[0] = 1; matrix->m[4] = 0; matrix->m[ 8] = 0; matrix->m[12] = 0;
    matrix->m[1] = 0; matrix->m[5] = 1; matrix->m[ 9] = 0; matrix->m[13] = 0;
    matrix->m[2] = 0; matrix->m[6] = 0; matrix->m[10] = 1; matrix->m[14] = 0;
    matrix->m[3] = 0; matrix->m[7] = 0; matrix->m[11] = 0; matrix->m[15] = 1;
}

void make_translation(matrix4f *matrix, vec3f t) {
    matrix->m[0] = 1; matrix->m[4] = 0; matrix->m[ 8] = 0; matrix->m[12] = t.x;
    matrix->m[1] = 0; matrix->m[5] = 1; matrix->m[ 9] = 0; matrix->m[13] = t.y;
    matrix->m[2] = 0; matrix->m[6] = 0; matrix->m[10] = 1; matrix->m[14] = t.z;
    matrix->m[3] = 0; matrix->m[7] = 0; matrix->m[11] = 0; matrix->m[15] = 1;
}


vec4f make_vec4f(float x, float y, float z, float w) {
    vec4f out;
    out.x = x; out.y = y; out.z = z; out.w = w;
    return out;
}

vec4f row(matrix4f* matrix, int i) {
    vec4f out;
    out.x = matrix->m[i]; out.y = matrix->m[i + 4 * 1]; out.z = matrix->m[i + 4 * 2]; out.w = matrix->m[i + 4 * 3]; 
    return out;    
}
vec4f column(matrix4f* matrix, int i) {
    vec4f out;
    out.x = matrix->m[i * 4]; out.y = matrix->m[i * 4 + 1]; out.z = matrix->m[i * 4 + 2]; out.w = matrix->m[i * 4 + 3];
    return out;
}

float dot(vec4f a, vec4f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

void multiply(matrix4f *a, matrix4f *b, matrix4f *out) {
    int i, j;
    vec4f tmp;
    for (j = 0; j < 4; j++) {
        tmp = column(b, j);
        for (i = 0; i < 4; i++) {
            out->m[j * 4 + i] = dot(row(a, i), tmp);
        }
    }
}

vec3f add(vec3f a, vec3f b) {
    vec3f out;
    out.x = a.x + b.x; out.y = a.y + b.y; out.z = a.z + b.z;
    return out;
}

vec4f transform_vector(matrix4f *m, vec4f v) {
    vec4f out;
    out.x = dot(row(m, 0), v);
    out.y = dot(row(m, 1), v);
    out.z = dot(row(m, 2), v);
    out.w = dot(row(m, 3), v);
    return out;
}

float invf(int i, int j, const float* m) {
    int o = 2 + (j - i);
    float inv;
    i += 4 + o;
    j += 4 - o;

    #define e(a,b) m[((j + b) % 4) * 4 + ((i + a) % 4)]

    inv =
        +e(+1, -1) * e(+0, +0) * e(-1, +1)
        +e(+1, +1) * e(+0, -1) * e(-1, +0)
        +e(-1, -1) * e(+1, +0) * e(+0, +1)
        -e(-1, -1) * e(+0, +0) * e(+1, +1)
        -e(-1, +1) * e(+0, -1) * e(+1, +0)
        -e(+1, -1) * e(-1, +0) * e(+0, +1);

    return (o % 2) ? inv : -inv;
    #undef e
}

bool invert4f(matrix4f *matrix, matrix4f *out) {
    int i, j;
    float inv[16];
    double D = 0;
    float *m = matrix->m;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            inv[j * 4 + i] = invf(i, j, m);

    for(int k=0;k<4;k++) D += m[k] * inv[k*4];

    if (D == 0) return false;

    D = 1.0 / D;

    for (int i = 0; i < 16; i++)
        out->m[i] = inv[i] * D;

    return true;
}

matrix4f transpose4f(matrix4f *matrix) {
    int i, j;
    matrix4f out;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            out.m[j * 4 + i] = matrix->m[i * 4 + j];
    return out;
}