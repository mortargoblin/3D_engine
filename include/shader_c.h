#ifndef SHADER_C_H
#define SHADER_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>

GLuint LoadShaders_C(const char* vertex_path,
                     const char* fragment_path);

#ifdef __cplusplus
}
#endif

#endif

