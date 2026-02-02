#include "model.h"

void model_init(Model *model, Mesh *mesh) {
  model->mesh = mesh;
  glm_mat4_identity(model->transform);
}

void model_set_position(Model *model, vec3 pos) {
  glm_mat4_identity(model->transform);
  glm_translate(model->transform, pos);
}

void model_draw(const Model *model, GLuint shader_program) {
  GLuint modelLoc = glGetUniformLocation(shader_program, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model->transform[0][0]);

  mesh_draw(model->mesh);
}

