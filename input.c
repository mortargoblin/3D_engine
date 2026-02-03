#include "app.h"
#include "input.h"

#include <string.h>

static double last_mouse_x = 0.0;
static double last_mouse_y = 0.0;
static bool first_mouse = true;

void input_init(GLFWwindow *window) {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
}

void input_update(InputState *out, GLFWwindow *window, float delta_time) {
  memset(out, 0, sizeof(*out));

  //// Mouse ////
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  if (first_mouse) {
    last_mouse_x = x;
    last_mouse_y = y;
    first_mouse = false;
  }

  double dx = x - last_mouse_x;
  double dy = y - last_mouse_y;

  last_mouse_x = x;
  last_mouse_y = y;

  float mouse_sensitivity = 0.0025f;
  out->yaw   = -(float)dx * mouse_sensitivity;
  out->pitch = -(float)dy * mouse_sensitivity;
  // printf("yaw: %f\n", out->yaw);

  //// Keyboard ////
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    out->move[2] -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    out->move[2] += 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    out->move[0] -= 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    out->move[0] += 1.0f;
  }

  out->sprint = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
}

