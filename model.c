#include "model.h"

void model_init(Model *model, Mesh *mesh) {
  model->mesh = mesh;
  glm_mat4_identity(model->transform);
}

Model model_create(Mesh *mesh) {
  Model model = {0};
  model.mesh = mesh;
  glm_mat4_identity(model.transform);

  return model;
}

void model_set_position(Model *model, vec3 pos) {
  glm_mat4_identity(model->transform);
  glm_translate(model->transform, pos);
}

void model_draw(const Model *model, GLuint shader_program) {
  GLuint model_location = glGetUniformLocation(
      shader_program, 
      "model"
      );
  glUniformMatrix4fv(
      model_location, 
      1, 
      GL_FALSE, 
      &model->transform[0][0]
      );

  // mesh_draw(model->mesh);
  glBindVertexArray(model->mesh->vao);
  glDrawElements(
      GL_TRIANGLES, 
      model->mesh->index_count, 
      GL_UNSIGNED_INT, 0
      );

  glBindVertexArray(0);
  // 
}

