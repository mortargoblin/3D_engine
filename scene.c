#include "scene.h"

Scene scene_create(int model_count, Model *models) {
  Scene scene = {0};
  scene.model_count = model_count;
  // printf("MODEL COUNT ACCORDING TO SCENE: %d", sizeof(*models));
  scene.models = malloc(model_count * sizeof(Model));

  for (int i = 0; i < model_count; i++) {
    scene.models[i] = models[i];
  }

  return scene;
}
