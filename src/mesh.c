#include "mesh.h"

void destroy_mesh(mesh *mesh) {
    free(mesh->vertices);
    free(mesh->faces);
}
