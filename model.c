#include "model.h"

void model_init(Model *model, Mesh *mesh) {
  model->mesh = mesh;
  glm_mat4_identity(model->transform);
}

Model model_create(Mesh *mesh) {
  Model model = {0};
  model.mesh = mesh;

  // glm-way of assigning vectors
  glm_mat4_identity(model.transform);
  glm_vec3_zero(model.position);
  glm_vec3_one(model.scale);

  return model;
}

void model_transform_update(Model *model) {
  glm_mat4_identity(model->transform);

  glm_translate(model->transform, model->position);
  glm_rotate_y(model->transform, model->rotation[1], model->transform);
  glm_scale(model->transform, model->scale);
}

void model_set_position(Model *model, vec3 pos) {
  glm_vec3_copy(pos, model->position);
}

void model_spin(Model *model, float angle) {
  model->rotation[1] += angle;
}

void model_set_rotation(Model *model, float angle) {
  model->rotation[1] = angle;
}

void model_draw(Model *model, GLuint shader_program) {
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

