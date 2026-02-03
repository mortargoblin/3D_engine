#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "include/cglm/cglm.h"

typedef struct {
    Mesh *mesh; // shared mesh
    mat4 transform; // model matrix
} Model;

void model_init(Model *model, Mesh *mesh);
void model_set_position(Model *model, vec3 pos);
void model_spin(Model *model, float angle);
void model_draw(Model *model, GLuint shader_program);

Model model_create(Mesh *mesh);

#endif // MODEL_H
