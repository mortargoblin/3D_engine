#ifndef SCENE_H
#define SCENE_H

#include "model.h"

typedef struct {
  int model_count;
  Model models[];
  // some other stuff maybe idk
} Scene;

Scene scene_create(int model_count, Model models[]);

#endif // SCENE_H
