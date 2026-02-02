// 3D Engine MAIN.C
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "app.h"
#include "input.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "renderer.h"

int main() {

  srand(time(NULL));

  App app = {0};

  // 1024 768
  if (!app_init(&app, 1920, 1080, "3D Engine")) {
    return -1;
  }

  InputState input;

  Camera camera = camera_create(
      (vec3){0.0f, 0.0f, 5.0f},
      (float)app.screen_width / app.screen_height
      );

  input_init(app.window);
  renderer_init();

  Mesh scene_meshes[] = {
    mesh_load_obj("assets/120KRH92.obj"),
    mesh_load_obj("assets/hellcat.obj")
  };

  int mesh_count = 2;
  for (int i = 0; i < mesh_count; i++) {
    mesh_generate_random_colors(&scene_meshes[i]);
  }

  Model models[mesh_count * sizeof(Model)];

  for (int i = 0; i < mesh_count; i++) {
    models[i] = model_create(&scene_meshes[i]);
  }

  /* TODO: FIX
  Scene scene = scene_create(2, models);
  if (scene) {
    printf("wtf\n");
  }
  printf("SCENE: %d\n", scene.model_count);
  */

  /////// MAIN LOOP ///////
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

    renderer_draw(models, &camera);

    app_swap_buffers(&app);

  }

  app_shutdown(&app);
  printf("done\n");
  return 0;
}
