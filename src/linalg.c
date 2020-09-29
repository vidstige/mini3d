#include "linalg.h"

vec3f make_vec3f(float x, float y, float z) {
    vec3f out;
    out.x = x; out.y = y; out.z = z;
    return out;
}

void identity(matrix3 *matrix) {
    matrix->m[0] = 1; matrix->m[3] = 0; matrix->m[6] = 0;
    matrix->m[1] = 0; matrix->m[4] = 1; matrix->m[7] = 0;
    matrix->m[2] = 0; matrix->m[5] = 0; matrix->m[8] = 1;
}
vec3f row(matrix3* matrix, int i) {
    vec3f out;
    out.x = matrix->m[i]; out.y = matrix->m[i+3*1]; out.z = matrix->m[i+3*2];
    return out;    
}
vec3f column(matrix3* matrix, int i) {
    vec3f out;
    out.x = matrix->m[i*3]; out.y = matrix->m[i*3+1]; out.z = matrix->m[i*3+2];
    return out;
}

float dot(vec3f a, vec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void multiply(matrix3 *a, matrix3 *b, matrix3 *out) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out->m[j*3 + i] = dot(row(a, i), column(b, j));
        }
    }
}
vec3f add(vec3f a, vec3f b) {
    vec3f out;
    out.x = a.x + b.x; out.y = a.y + b.y; out.z = a.z + b.z;
    return out;
}
vec3f transform(matrix3 *m, vec3f t, vec3f v) {
    vec3f out;
    out.x = dot(row(m, 0), v);
    out.y = dot(row(m, 1), v);
    out.z = dot(row(m, 2), v);
    return add(out, t);
}
