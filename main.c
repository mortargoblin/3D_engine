// 3D Engine MAIN.C
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "app.h"
#include "input.h"
#include "camera.h"
#include "mesh.h"
#include "renderer.h"

int main() {

  srand(time(NULL));

  App app = {0};

  // 1024 768
  if (!app_init(&app, 1024, 768, "3D Engine")) {
    return -1;
  }

  InputState input;

  Camera camera = camera_create(
      (vec3){0.0f, 0.0f, 5.0f},
      (float)app.screen_width / app.screen_height
      );

  input_init(app.window);
  renderer_init();

  Mesh mesh = mesh_load_obj("assets/120KRH92.obj");

  mesh_generate_random_colors(&mesh);

  /////// MAIN LOOP ///////
  double current_time = glfwGetTime();
  while(!app_should_close(&app)) {

    app_update_time(&app);

    input_update(
        &input, 
        app.window, 
        app.delta_time
        );

    camera_update(
        &camera,
        &input,
        app.delta_time,
        (float)app.screen_width / app.screen_height
        );

    renderer_draw(&mesh, &camera);

    app_swap_buffers(&app);

  }

  app_shutdown(&app);
  printf("done\n");
  return 0;
}
