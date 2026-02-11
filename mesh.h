#ifndef MESH_H
#define MESH_H

#include <stdlib.h>
#include <stdio.h>

#include <stddef.h>
#include <GL/glew.h>

typedef struct {
    GLuint vao;
    GLuint vbo;
    // colors are owned by mesh
    GLuint vbo_colors;
    size_t vertex_count;
    GLuint ebo;
    size_t index_count;
} Mesh;

Mesh mesh_load_obj(const char *path);

void mesh_set_color(Mesh *mesh, float colors[3]);

void mesh_generate_random_colors(Mesh *mesh);

void mesh_bind_colors(Mesh *mesh, GLfloat *colors);

void mesh_draw(const Mesh *mesh);

void mesh_destroy(Mesh *mesh);

#endif // MESH_H

