#include "app.h"

#include <stdio.h>

bool app_init(App *app, int width, int height, const char *title) {
  if (!glfwInit()) {
    fprintf(stderr, "GLFW init failed\n");
    return false;
  }

  // libdecor bug would cause weird window resize on my compositor (hyprland)
  glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);

  glfwWindowHint(GLFW_SAMPLES, 4);
  // opengl version 3.3 -> both major and minor 3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  // prevent old opengl
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  app->window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!app->window) {
    fprintf(stderr, "Failed to create window\n");
    glfwTerminate();
    return false;
  }

  // init glew
  glfwMakeContextCurrent(app->window);
  glewExperimental = false;

  // TODO: fix 
  // glew wayland bug (or something) returns 4
  GLenum err = glewInit();
  if (err != GLEW_OK && err != 4) {
    printf("%d", err);
    fprintf(stderr, "GLEW init failed\n");
    return false;
  }

  // input mode. captures esc etc
  glfwSetInputMode(app->window, GLFW_STICKY_KEYS, GL_TRUE);

  // update app struct
  app->screen_width  = width;
  app->screen_height = height;
  app->time = glfwGetTime();
  app->delta_time = 0.0f;

  return true;
}

void app_update_time(App *app) {
  double new_time = glfwGetTime();
  app->delta_time = (float)(new_time - app->time);
  app->time = new_time;
}

bool app_should_close(const App *app) {
  return glfwWindowShouldClose(app->window);
}

void app_swap_buffers(App *app) {
  glfwSwapBuffers(app->window);
  glfwPollEvents();
}

void app_shutdown(App *app) {
  if (app->window) {
    glfwDestroyWindow(app->window);
  }
  glfwTerminate();
}

