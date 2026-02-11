#include "scene.h"

Scene scene_create(int model_count, Model *models) {
  Scene scene = {0};
  scene.model_count = model_count;
  
  // TODO fix: perhaps this allocates too much memory
  scene.models = malloc(model_count * sizeof(Model));

  for (int i = 0; i < model_count; i++) {
    scene.models[i] = models[i];
  }

  return scene;
}

void scene_transform_update(Scene *scene) {
  for (int i = 0; i < scene->model_count; i++) {
    model_transform_update(&scene->models[i]);
  }
}

