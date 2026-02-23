// 3D Engine MAIN
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "app.h"
#include "input.h"
#include "camera.h"
#include "player.h"
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

  // create meshes
  int mesh_count = 3;
  Mesh scene_meshes[] = {
    mesh_load_obj("assets/elite_ship.obj"),
    mesh_load_obj("assets/simple_house.obj"),
    mesh_load_obj("assets/120KRH92.obj"),
  };

  // create models (mesh + transform etc)
  Model models[mesh_count];
  int model_count = 0;

  for (int i = 0; i < mesh_count; i++) {
    models[i] = model_create(&scene_meshes[i]);
    model_count++;
  }

  Scene scene = scene_create(model_count, models);

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
    
    // model_transform_update(&scene.models[1]);
    scene_transform_update(&scene);

    // transform logic

    model_set_position(&scene.models[0], (vec3) {
        0, // (cos(app.time * 2) * 2), 
        (cos(app.time * 2) * 2), 
        0 // (sin(app.time * 2) * 2), 
        });

    model_spin(&scene.models[0], 0.02);

    model_set_position(&scene.models[1], (vec3) {
        6,0,0
        });

    model_set_position(&scene.models[0], (vec3) {
        -5, // (cos(app.time * 2) * 2), 
        0, 
        -10 // (sin(app.time * 2) * 2), 
        });
    
    renderer_draw(&scene, &camera);
    app_swap_buffers(&app);
  }

  app_shutdown(&app);
  printf("done\n");
  return 0;
}
