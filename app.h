// APP.H
#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct App {
  GLFWwindow *window;
  double time;
  double delta_time;

  int screen_width;
  int screen_height;
} App;

// function declarations
bool app_init(App *app, int width, int height, const char *title);
void app_update_time(App *app);
bool app_should_close(const App *app);
void app_swap_buffers(App *app);
void app_shutdown(App *app);

#endif // APP_H
