#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "include/cglm/cglm.h"
#include "input.h"

typedef struct {
    vec3 position;

    float yaw;     // radians
    float pitch;   // radians

    float fov;     // degrees
    float speed;
    float sprint_multiplier;

    mat4 view;
    mat4 projection;
} Camera;

Camera camera_create(vec3 start_position, float aspect_ratio);

void camera_update(
    Camera *cam,
    const InputState *input,
    float delta_time,
    float aspect_ratio
);

void camera_get_forward(const Camera *cam, vec3 out);
void camera_get_right(const Camera *cam, vec3 out);

#endif // CAMERA_H

