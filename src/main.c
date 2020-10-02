#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "linalg.h"
#include "mesh.h"
#include "io.h"
#include "render.h"

void clear(uint32_t *buffer, uint32_t color) {
    size_t i;
    for (i = 0; i < 320 * 200; i++) {
        buffer[i] = color;
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

vec4f as_homogenous(vec3f v) {
    vec4f out;
    out.x = v.x; out.y = v.y; out.z = v.z; out.w = 1.f;
    return out;
}

// normal of triangle
vec4f normal_of_triangle(vec3f v0, vec3f v1, vec3f v2) {
    vec3f normal = normalized3f(cross(vec3f_subtract(v2, v0), vec3f_subtract(v1, v0)));
    return make_vec4f(normal.x, normal.y, normal.z, 0);
}

uint32_t rgb(uint32_t r, uint32_t g, uint32_t b) {
    const uint32_t alpha = 0xff;
    return alpha << 24 | (r & 0xff) << 16 | (g & 0xff) << 8 | (b & 0xff);
}

uint32_t rgbf(float r, float g, float b) {
    return rgb((uint32_t)(r * 255), (uint32_t)(g * 255), (uint32_t)(b * 255));
}

void print_vec3f(FILE* file, const char* label, vec3f v) {
    fprintf(file, "%s: %f %f %f\n", label, v.x, v.y, v.z);
}
void print_vec4f(FILE* file, const char* label, vec4f v) {
    fprintf(file, "%s: %f %f %f %f\n", label, v.x, v.y, v.z, v.w);
}

vec4f points[100];
void draw(uint32_t *buffer, mesh* mesh, float t) {
    float omega = 0.02f, intensity;
    int i;
    matrix4f tmp, mv, mvp, projection, nt;
    vec4f clip;
    vec3f ndc, screen;
    face face;
    vec4f normal;
    vec3f *transformed = malloc(sizeof(vec3f) * mesh->n_vertices);
    vec4f light = make_vec4f(0, 0, -1, 0);
    float *z_buffer = malloc(sizeof(float) * 320 * 200);

    for (i = 0; i < 320 * 200; i++) {
        z_buffer[i] = 100.f;
    }


    // Compute face normals
    vec4f *normals = malloc(sizeof(vec4f) * mesh->n_faces);
    for (i = 0; i < mesh->n_faces; i++) {
        face = mesh->faces[i];
        normals[i] = normal_of_triangle(mesh->vertices[face.i0], mesh->vertices[face.i1], mesh->vertices[face.i2]);
    }

    // compute mvp matrix
    make_identity(&mvp);
    rotate_y(&tmp, omega * t); multiply(&tmp, &mvp, &mvp);
    make_translation(&tmp, make_vec3f(0.f, 0.f, -3.f)); multiply(&tmp, &mvp, &mv);
    make_projection(&tmp, 90, 320.f/200.f, 0.1, 5); multiply(&tmp, &mv, &mvp);

    // compute normal transform
    invert4f(&mv, &tmp);
    nt = transpose4f(&tmp);

    // Transform vertices
    for (i = 0; i < mesh->n_vertices; i++) {
        clip = transform_vector(&mvp, as_homogenous(mesh->vertices[i]));
        ndc = divide(clip);
        screen.x = 160 + (int)(160.f * ndc.x);
        screen.y = 100 - (int)(100.f * ndc.y);
        screen.z = ndc.z;
        transformed[i] = screen;
    }

    for (i = 0; i < mesh->n_faces; i++) {
        face = mesh->faces[i];
        normal = transform_vector(&nt, normals[i]);
        //print_vec4f(stderr, "normal", normal);
        intensity = dot(normal, light);
        //fprintf(stderr, "intensity %f\n", intensity);
        if (normal.z < 0.f) {
            //intensity = 0.8;
            render_triangle(buffer, z_buffer, rgbf(intensity, intensity, intensity), transformed[face.i0], transformed[face.i1], transformed[face.i2]);
        }
    }
    free(z_buffer);
    free(normals);
    free(transformed);
}

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

    //FILE* f = fopen("meshes/cube.obj", "r");
    FILE* f = fopen("meshes/cute-darth-vader.obj", "r");
    mesh = load_obj(f);
    fclose(f);
    fprintf(stderr, "loaded %lu, %lu", mesh.n_vertices, mesh.n_faces);

    initialize();
    for (t = 0; t < 2 * M_PI / 0.02; t += 1.f) {
        clear(buffer, 0xff222222);
        draw(buffer, &mesh, t);
        fwrite(buffer, sizeof(uint32_t), 320 * 200, stdout);
    }

    destroy_mesh(&mesh);
}
