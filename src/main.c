#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "linalg.h"
#include "mesh.h"
#include "io.h"

void clear(uint32_t *buffer, uint32_t color) {
    size_t i;
    for (i = 0; i < 320 * 200; i++) {
        buffer[i] = color;
    }
}

void pixel(uint32_t *buffer, vec2i screen, uint32_t color) {
    int x = screen.x;
    int y = screen.y;
    if (x >= 0 && x < 320 && y >= 0 && y < 200) {
        buffer[x + y * 320] = color;
    }
}

void rotate_x(matrix3 *matrix, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    matrix->m[0] = 1; matrix->m[3] = 0; matrix->m[6] = 0;
    matrix->m[1] = 0; matrix->m[4] = c; matrix->m[7] = -s;
    matrix->m[2] = 0; matrix->m[5] = s; matrix->m[8] = c;
}
void rotate_y(matrix3 *matrix, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    matrix->m[0] = c; matrix->m[3] = 0; matrix->m[6] = -s;
    matrix->m[1] = 0; matrix->m[4] = 1; matrix->m[7] = 0;
    matrix->m[2] = s; matrix->m[5] = 0; matrix->m[8] = c;
}
void rotate_z(matrix3 *matrix, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    matrix->m[0] = c; matrix->m[3] = -s; matrix->m[6] = 0;
    matrix->m[1] = s; matrix->m[4] =  c; matrix->m[7] = 0;
    matrix->m[2] = 0; matrix->m[5] =  0; matrix->m[8] = 1;
}

vec2i project(vec3f v, int w, int h) {
    vec2i out;
    float fov = 0.01;
    out.x = w / 2 + (int)(v.x / (fov * v.z));
    out.y = h / 2 + (int)(v.y / (fov * v.z));
    return out;
}

vec3f points[100];
void draw(uint32_t *buffer, float t) {
    float omega = 0.02;
    int i;
    vec2i screen;
    matrix3 m;
    vec3f point, translation;
    rotate_y(&m, omega * t);
    translation.x = 0; translation.y = 0; translation.z = 120;
    for (i = 0; i < 100; i++) {
        point = transform(&m, translation, points[i]);
        screen = project(point, 320, 200);
        pixel(buffer, screen, 0xffffffff);
    }
}

/*
void draw_triangle(uint32_t *buffer, uint32_t color) {
}

void draw_mesh(uint32_t *buffer, mesh mesh) {
    int i;
    for (int i = 0; i < mesh.n_faces; i++) {
        draw_triangle(buffer, );
    }
}*/

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
    mesh mesh;

    FILE* f = fopen("meshes/cube.obj", "r");
    mesh = load_obj(f);
    fclose(f);

    //draw_mesh(buffer, mesh);

    destroy_mesh(&mesh);

    initialize();
    for (t = 0; t < M_PI / 0.02; t += 1.f) {
        clear(buffer, 0xff222222);
        draw(buffer, t);
        fwrite(buffer, sizeof(uint32_t), 320 * 200, stdout);
    }
}
