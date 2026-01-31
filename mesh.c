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

    // count total vertices 
    size_t vertex_count = 0;
    for (unsigned int i = 0; i < obj->face_count; i++) {
        // vertex_count += obj->face_vertices[i];
        vertex_count += (obj->face_vertices[i] - 2) * 3;
    }

    GLfloat *vertices = malloc(vertex_count * 3 * sizeof(GLfloat));

    size_t v = 0;
    size_t index_offset = 0;

    // assign verticies to vao
    for (unsigned int f = 0; f < obj->face_count; f++) {
        for (unsigned int fv = 0; fv < obj->face_vertices[f]; fv++) {
            fastObjIndex idx = obj->indices[index_offset + fv];

            vertices[v++] = obj->positions[idx.p * 3 + 0];
            vertices[v++] = obj->positions[idx.p * 3 + 1];
            vertices[v++] = obj->positions[idx.p * 3 + 2];
        }
        index_offset += obj->face_vertices[f];
    }

    fast_obj_destroy(obj);

    // opengl buffers
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    /*
    glBufferData(
        GL_ARRAY_BUFFER,
        obj->position_count * sizeof(GLfloat),
        obj->positions,
        GL_STATIC_DRAW
    );
    */
    glBufferData(
        GL_ARRAY_BUFFER,
        v * sizeof(GLfloat),
        vertices,
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0
    );

    glBindVertexArray(0);
    free(vertices);

    mesh.vertex_count = v / 3;
    return mesh;
}

void mesh_draw(const Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
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
    glVertexAttribPointer(
        1,        // attribute location
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0
    );

    glBindVertexArray(0);
    free(colors);
}

void mesh_destroy(Mesh *mesh) {
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
}

