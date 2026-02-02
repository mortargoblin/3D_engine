#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include "include/cglm/cglm.h"

typedef struct {
  Mesh mesh;
  vec3 transform;
} Entity;

#endif // ENTITY_H
