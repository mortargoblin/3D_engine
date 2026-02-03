#ifndef RENDERER_H
#define RENDERER_H

#include "mesh.h"
#include "camera.h"
#include "scene.h"

void renderer_init(void);
void renderer_draw(Scene *scene, const Camera *camera);
void renderer_clear(void);
void renderer_shutdown(void);

#endif // RENDERER_H

