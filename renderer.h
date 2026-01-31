#ifndef RENDERER_H
#define RENDERER_H

#include "mesh.h"
#include "camera.h"

void renderer_init(void);
void renderer_draw(const Mesh *mesh, const Camera *camera);
void renderer_shutdown(void);

#endif // RENDERER_H

