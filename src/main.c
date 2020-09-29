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

void rotate_x(matrix4f *matrix, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    matrix->m[0] = 1; matrix->m[4] = 0; matrix->m[ 8] =  0; matrix->m[12] = 0;
    matrix->m[1] = 0; matrix->m[5] = c; matrix->m[ 9] = -s; matrix->m[13] = 0;
    matrix->m[2] = 0; matrix->m[6] = s; matrix->m[10] =  c; matrix->m[14] = 0;
    matrix->m[3] = 0; matrix->m[7] = 0; matrix->m[11] =  0; matrix->m[15] = 1;
}
void rotate_y(matrix4f *matrix, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    matrix->m[0] = c; matrix->m[4] = 0; matrix->m[ 8] = -s; matrix->m[12] = 0;
    matrix->m[1] = 0; matrix->m[5] = 1; matrix->m[ 9] =  0; matrix->m[13] = 0;
    matrix->m[2] = s; matrix->m[6] = 0; matrix->m[10] =  c; matrix->m[14] = 0;
    matrix->m[3] = 0; matrix->m[7] = 0; matrix->m[11] =  0; matrix->m[15] = 1;
}
void rotate_z(matrix4f *matrix, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    matrix->m[0] = c; matrix->m[4] = -s; matrix->m[ 8] = 0; matrix->m[12] = 0;
    matrix->m[1] = s; matrix->m[5] =  c; matrix->m[ 9] = 0; matrix->m[13] = 0;
    matrix->m[2] = 0; matrix->m[6] =  0; matrix->m[10] = 1; matrix->m[14] = 0;
    matrix->m[3] = 0; matrix->m[7] =  0; matrix->m[11] = 0; matrix->m[15] = 1;
}

float degrees_to_radians(float degrees) {
    return 2.f * M_PI * degrees / 360.f;
}

void make_projection(matrix4f *matrix, float fovy, float aspect, float near, float far) {
    float s = 1.f / tan(degrees_to_radians(fovy) / 2.f);
    float sx = s / aspect, sy = s;
    float zz = (far + near) / (near - far);
    float zw = 2.f * far * near / (near - far);

    matrix->m[0] = sx; matrix->m[4] =  0; matrix->m[ 8] =  0; matrix->m[12] =  0;
    matrix->m[1] =  0; matrix->m[5] = sy; matrix->m[ 9] =  0; matrix->m[13] =  0;
    matrix->m[2] =  0; matrix->m[6] =  0; matrix->m[10] = zz; matrix->m[14] = -1;
    matrix->m[3] =  0; matrix->m[7] =  0; matrix->m[11] = zw; matrix->m[15] =  0;
}

vec3f divide(vec4f v) {
    return make_vec3f(v.x / v.w, v.y / v.w, v.z / v.w);
}

vec4f points[100];
void draw(uint32_t *buffer, float t) {
    float omega = 0.02f;
    int i;
    matrix4f tmp, mvp, projection;
    vec4f clip;
    vec3f screen;
    vec2i screen_i;

    make_identity(&mvp);
    rotate_y(&tmp, omega * t); multiply(&tmp, &mvp, &mvp);
    make_translation(&tmp, make_vec3f(0.f, 0.f, 4.f)); multiply(&tmp, &mvp, &mvp);
    make_projection(&tmp, 90, 320.f/200.f, 0.1, 5); multiply(&tmp, &mvp, &mvp);

    for (i = 0; i < 100; i++) {
        clip = transform_vector(&mvp, points[i]);
        screen = divide(clip);
        screen_i.x = 160 + (int)(160.f * screen.x);
        screen_i.y = 100 - (int)(100.f  *screen.y);
        pixel(buffer, screen_i, 0xffffffff);
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
    float s = 1.f;
    for (i = 0; i < 100; i++) {
        points[i].x = -s / 2.f + s * randf();
        points[i].y = -s / 2.f + s * randf();
        points[i].z = -s / 2.f + s * randf();
        points[i].w = 1.f;
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
    for (t = 0; t < 2 * M_PI / 0.02; t += 1.f) {
        clear(buffer, 0xff222222);
        draw(buffer, t);
        fwrite(buffer, sizeof(uint32_t), 320 * 200, stdout);
    }
}
