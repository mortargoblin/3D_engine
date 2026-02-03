#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include "mesh.h"

typedef struct {
  int model_count;
  Model *models;
  // some other stuff maybe idk
} Scene;

Scene scene_create(
    int model_count, Model models[]
    );

void scene_transform_update(Scene *scene);

#endif // SCENE_H
