#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "include/cglm/cglm.h"

typedef struct {
    Mesh *mesh; // shared mesh
    vec3 position;
    vec3 rotation; // either euler or quaternion
    vec3 scale;
    mat4 transform; // model matrix
} Model;

void model_init(Model *model, Mesh *mesh);
void model_transform_update(Model *model);
// void model_transform_init(Model *model);
void model_set_position(Model *model, vec3 position);
void model_move(Model *model, vec3 direction);
void model_spin(Model *model, float angle);
void model_set_rotation(Model *model, float angle);
void model_draw(Model *model, GLuint shader_program);

Model model_create(Mesh *mesh);

#endif // MODEL_H
