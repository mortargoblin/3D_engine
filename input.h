// INPUT.H
#ifndef INPUT_H
#define INPUT_H

#include "app.h"
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "./include/cglm/cglm.h"

typedef struct {
  vec3 move;
  float yaw;
  float pitch;
  bool sprint;
} InputState;

void input_init(GLFWwindow *window);

void input_update(
    InputState *out, 
    GLFWwindow *window,
    float delta_time
    );

#endif // INPUT_H
