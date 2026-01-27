#include "shader_c.h"

// Forward declaration of the C++ function
GLuint LoadShaders(const char* vertex_file_path,
                   const char* fragment_file_path);

GLuint LoadShaders_C(const char* vertex_path,
                     const char* fragment_path)
{
    return LoadShaders(vertex_path, fragment_path);
}

