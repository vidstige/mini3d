#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void clear(uint32_t *buffer, uint32_t color) {
    size_t i;
    for (i = 0; i < 320 * 200; i++) {
        buffer[i] = color;
    }
}

typedef struct { float x, y, z; } vec3;
typedef struct {
    float m[9];
} matrix3;


void identity(matrix3 *matrix) {
    matrix->m[0] = 1; matrix->m[3] = 0; matrix->m[6] = 0;
    matrix->m[1] = 0; matrix->m[4] = 1; matrix->m[7] = 0;
    matrix->m[2] = 0; matrix->m[5] = 0; matrix->m[8] = 1;
}
vec3 row(matrix3* matrix, int i) {
    vec3 out;
    out.x = matrix->m[i]; out.y = matrix->m[i+3*1]; out.z = matrix->m[i+3*2];
    return out;    
}
vec3 column(matrix3* matrix, int i) {
    vec3 out;
    out.x = matrix->m[i*3]; out.y = matrix->m[i*3+1]; out.z = matrix->m[i*3+2];
    return out;
}

float dot(vec3 a, vec3 b) {
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
vec3 transform(matrix3 *m, vec3 v) {
    vec3 out;
    out.x = dot(row(m, 0), v);
    out.y = dot(row(m, 1), v);
    out.z = dot(row(m, 2), v);
    return out;
}

void pixel(uint32_t *buffer, int x, int y, uint32_t color) {
    if (x >= 0 && x < 320 && y >= 0 && y < 200) {
        buffer[x + y * 320] = color;
    }
}

vec3 points[100];
void draw(uint32_t *buffer, float t) {
    int i, sx, sy;
    matrix3 m;
    vec3 tmp;
    identity(&m);
    for (i = 0; i < 100; i++) {
        tmp = transform(&m, points[i]);
        sx = 160 + (int)tmp.x;
        sy = 100 + (int)tmp.y;
        pixel(buffer, sx, sy, 0xffffffff);
    }
}

float randf() {
    return (float)rand() / (float)(RAND_MAX);
}

void initialize() {
    int i;
    float s = 100.f;
    for (i = 0; i < 100; i++) {
        points[i].x = -s / 2.f + s * randf();
        points[i].y = -s / 2.f + s * randf();
        points[i].z = -s / 2.f + s * randf();
    }
}

int main() {
    uint32_t buffer[320 * 200];
    float t = 0;
    float dt = 1;

    initialize();

    while (1) {
        clear(buffer, 0xff222222);
        draw(buffer, t);
        fwrite(buffer, sizeof(uint32_t), 320 * 200, stdout);
        t += dt;
    }
}
