#include <stdio.h>
#include "mesh.h"

mesh load_obj(FILE* file)
{
    char line[256];
    size_t vertices_capacity = 1024, vertices_i = 0;
    vec3f *vertices = malloc(sizeof(vec3f) * vertices_capacity);
    size_t faces_capacity = 1024, faces_i = 0;
    face* faces = malloc(sizeof(face) * faces_capacity);
    vec3f vertex;
    face face;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue;
        
        if (sscanf(line, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z)) {
            vertices[vertices_i++] = vertex;
        }
        if (sscanf(line, "f %lu/%*d/%*d %lu/%*d/%*d %lu/%*d/%*d\n", &face.i0, &face.i1, &face.i2)) {
            face.i0--; face.i1--; face.i2--;            
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