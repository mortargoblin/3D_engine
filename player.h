#ifndef PLAYER_H
#define PLAYER_H

#include "include/cglm/cglm.h"
#include "input.h"

typedef struct {
  vec3 position;

  float yaw;     // radians
  float pitch;   // radians

} Player;

void player_update(Player *player, InputState *input);

#endif // PLAYER_H
