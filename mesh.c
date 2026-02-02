#include "mesh.h"

#include <stdlib.h>
#include <stdio.h>

#include "include/fast_obj-1.3/fast_obj.h"
#include "include/fast_obj-1.3/fast_obj.c"

Mesh mesh_load_obj(const char *path) {
  Mesh mesh = {0};

  fastObjMesh *obj = fast_obj_read(path);
  if (!obj) {
    fprintf(stderr, "Failed to load OBJ: %s\n", path);
    return mesh;
  }

  // build indicies
  size_t index_count = 0;
  for (unsigned int i = 0; i < obj->face_count; i++) {
    index_count += (obj->face_vertices[i] - 2) * 3;
  }

  GLuint *indices = malloc(index_count * sizeof(GLuint));
  size_t idx = 0;
  size_t offset = 0;

  for (unsigned int f = 0; f < obj->face_count; f++) {
    unsigned int fv = obj->face_vertices[f];
    for (unsigned int i = 1; i + 1 < fv; i++) {
      indices[idx++] = obj->indices[offset + 0].p;
      indices[idx++] = obj->indices[offset + i].p;
      indices[idx++] = obj->indices[offset + i + 1].p;
    }
    offset += fv;
  }

  // opengl buffers
  glGenVertexArrays(1, &mesh.vao);
  glBindVertexArray(mesh.vao);

  glGenBuffers(1, &mesh.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(
      GL_ARRAY_BUFFER,
      obj->position_count * 3 * sizeof(float),
      obj->positions,
      GL_STATIC_DRAW
      );

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glGenBuffers(1, &mesh.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      index_count * sizeof(GLuint),
      indices,
      GL_STATIC_DRAW
      );

  glBindVertexArray(0);

  mesh.vertex_count = obj->position_count;
  mesh.index_count  = index_count;

  fast_obj_destroy(obj);
  free(indices);

  return mesh;
}


void mesh_draw(const Mesh *mesh) {
  glBindVertexArray(mesh->vao);
  glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void mesh_generate_random_colors(Mesh *mesh) {
  GLfloat *colors = malloc(mesh->vertex_count * 3 * sizeof(GLfloat));

  for (size_t i = 0; i < mesh->vertex_count; i++) {
    colors[i * 3 + 0] = (float)rand() / RAND_MAX;
    colors[i * 3 + 1] = (float)rand() / RAND_MAX * 0.5f;
    colors[i * 3 + 2] = (float)rand() / RAND_MAX * 0.5f;
  }

  glBindVertexArray(mesh->vao);

  glGenBuffers(1, &mesh->vbo_colors);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_colors);
  glBufferData(
      GL_ARRAY_BUFFER,
      mesh->vertex_count * 3 * sizeof(GLfloat),
      colors,
      GL_STATIC_DRAW
      );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindVertexArray(0);
  free(colors);
}

void mesh_destroy(Mesh *mesh) {
  glDeleteBuffers(1, &mesh->vbo);
  glDeleteVertexArrays(1, &mesh->vao);
}

