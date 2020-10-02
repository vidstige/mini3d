#include <stdio.h>
#include "mesh.h"

#define CAPACITY_STEP 1024

mesh load_obj(FILE* file)
{
    char line[256];
    size_t vertices_capacity = CAPACITY_STEP, vertices_i = 0;
    vec3f *vertices = malloc(sizeof(vec3f) * vertices_capacity);
    size_t faces_capacity = CAPACITY_STEP, faces_i = 0;
    face* faces = malloc(sizeof(face) * faces_capacity);
    vec3f vertex;
    face face;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue;
        
        if (sscanf(line, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z)) {
            // Increase buffer if needed
            if (vertices_i >= vertices_capacity) {
                vertices_capacity += CAPACITY_STEP;
                vertices = realloc(vertices, sizeof(vec3f) * vertices_capacity);
            }
            vertices[vertices_i++] = vertex;
        }
        if (sscanf(line, "f %lu//%*d %lu//%*d %lu//%*d\n", &face.i0, &face.i1, &face.i2)) {
            if (faces_i >= faces_capacity) {
                faces_capacity += CAPACITY_STEP;
                faces = realloc(faces, sizeof(face) * faces_capacity);
            }
            face.i0--; face.i1--; face.i2--;            
            if (face.i0 > vertices_i) fprintf(stderr, "%lu\n", faces_i);
            if (face.i1 > vertices_i) fprintf(stderr, "%lu\n", faces_i);
            if (face.i2 > vertices_i) fprintf(stderr, "%lu\n", faces_i);
            faces[faces_i++] = face;
        }
    }
    // Trim buffers
    vertices = realloc(vertices, sizeof(vec3f) * vertices_i);
    faces = realloc(faces, sizeof(face) * faces_i);
    
    mesh out;
    out.n_vertices = vertices_i;
    out.vertices = vertices;
    out.n_faces = faces_i;
    out.faces = faces;
    return out;
}

struct stl_triangle {
    float x0, y0, z0;
    float x1, y1, z1;
    float x2, y2, z2;
    uint16_t abc;
};

uint32_t swap(uint32_t num) {
    return ((num>>24)&0xff) | // move byte 3 to byte 0
           ((num<<8)&0xff0000) | // move byte 1 to byte 2
           ((num>>8)&0xff00) | // move byte 2 to byte 1
           ((num<<24)&0xff000000); // byte 0 to byte 3
}

mesh load_stl(FILE *file) {
    char header[80];
    int i;
    struct stl_triangle t;
    mesh out;
    size_t vertices_capacity = 16*1024, vertices_i = 0;
    size_t faces_capacity = 16*1024, faces_i = 0;
    uint32_t n_faces;
    out.vertices = malloc(sizeof(vec3f) * vertices_capacity);
    out.faces = malloc(sizeof(face) * faces_capacity);

    fread(header, sizeof(header), 1, file);
    header[79] = 0;
    fread(&n_faces, sizeof(n_faces), 1, file);
    out.n_faces = n_faces;
    for (i = 0; i < out.n_faces; i++) {
        fread(&t, sizeof(t), 1, file);
        out.vertices[vertices_i++] = make_vec3f(t.x0, t.y0, t.z0); out.faces[faces_i].i0 = vertices_i;
        out.vertices[vertices_i++] = make_vec3f(t.x1, t.y1, t.z1); out.faces[faces_i].i1 = vertices_i;
        out.vertices[vertices_i++] = make_vec3f(t.x2, t.y2, t.z2); out.faces[faces_i].i2 = vertices_i;
        faces_i++;
    }
    
    // Trim buffers
    out.vertices = realloc(out.vertices, sizeof(vec3f) * vertices_i);
    out.faces = realloc(out.faces, sizeof(face) * faces_i);

    return out;
}
