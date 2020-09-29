#include "linalg.h"

vec3f make_vec3f(float x, float y, float z) {
    vec3f out;
    out.x = x; out.y = y; out.z = z;
    return out;
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
