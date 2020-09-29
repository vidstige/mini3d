#ifndef MINI3D_MESH_H
#define MINI3D_MESH_H

#include <stdlib.h>
#include <stdint.h>
#include "linalg.h"

typedef struct {
    size_t i0, i1, i2;    
} face;

typedef struct {
    size_t n_vertices;
    vec3f* vertices;
    
    size_t n_faces;
    face* faces;
} mesh;

void destroy_mesh(mesh *mesh);

#endif