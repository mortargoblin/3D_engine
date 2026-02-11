#include "renderer.h"

#include <GL/glew.h>
#include <stdio.h>

#include "include/shader_c.h"

static GLuint program = 0;
static GLuint mvp_location = 0;

void renderer_init(void) {

  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  program = LoadShaders_C(
      "./SimpleVertexShader.glsl",
      "./SimpleFragmentShader.glsl"
      );

  if (!program) {
    fprintf(stderr, "Failed to load shaders\n");
  }

  mvp_location = glGetUniformLocation(program, "MVP");
}

void renderer_draw(Scene *scene, const Camera *camera) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);

  mat4 mvp;
  glm_mat4_mulN(
      (mat4 *[]){
      (mat4 *)&camera->projection,
      (mat4 *)&camera->view
      },
      2,
      mvp
      );

  glUniformMatrix4fv(
      mvp_location,
      1,
      GL_FALSE,
      &mvp[0][0]
      );

  for (int i = 0; i < scene->model_count; i++) {
    // what the fuuck
    // mesh_draw(scene->models[i].mesh);
    model_draw(&scene->models[i], program);
  }
}

void renderer_clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_shutdown(void) {
  glDeleteProgram(program);
}

