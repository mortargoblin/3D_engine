#include "camera.h"

#include <math.h>

#define MAX_PITCH 1.5f   // ~86 degrees

static void clamp_pitch(float *pitch) {
  if (*pitch > MAX_PITCH)  *pitch = MAX_PITCH;
  if (*pitch < -MAX_PITCH) *pitch = -MAX_PITCH;
}

Camera camera_create(vec3 start_position, float aspect_ratio) {
  Camera cam = {0};

  glm_vec3_copy(start_position, cam.position);

  cam.yaw   = glm_rad(180.0f);
  cam.pitch = 0.0f;

  cam.fov = 90.0f;
  cam.speed = 3.0f;
  cam.sprint_multiplier = 2.0f;

  glm_perspective(
      glm_rad(cam.fov),
      aspect_ratio,
      0.1f,
      100.0f,
      cam.projection
      );

  return cam;
}

void camera_update(
    Camera *cam,
    const InputState *input,
    float delta_time,
    float aspect_ratio
    ) {
  // rotation
  cam->yaw   += input->yaw;
  cam->pitch += input->pitch;
  clamp_pitch(&cam->pitch);

  // prevent float overflow
  if (cam->yaw > GLM_PI * 2.0f) cam->yaw -= GLM_PI * 2.0f;
  if (cam->yaw < 0.0f)          cam->yaw += GLM_PI * 2.0f;

  // direction vectors
  vec3 forward = {
    cosf(cam->pitch) * sinf(cam->yaw),
    sinf(cam->pitch),
    cosf(cam->pitch) * cosf(cam->yaw)
  };

  vec3 right = {
    sinf(cam->yaw - GLM_PI_2),
    0.0f,
    cosf(cam->yaw - GLM_PI_2)
  };

  vec3 up;
  glm_cross(right, forward, up);

  //// Movement ////
  float velocity = cam->speed * delta_time;
  if (input->sprint) {
    velocity *= cam->sprint_multiplier;
  }

  if (glm_vec3_norm(input->move) > 0.0f) {
    vec3 move = {0};
    glm_vec3_scale(forward, -input->move[2], move);
    glm_vec3_muladds(right, input->move[0], move);
    glm_vec3_normalize(move);
    glm_vec3_scale(move, velocity, move);
    glm_vec3_add(cam->position, move, cam->position);
  }

  //// View matrix ////
  vec3 target;
  glm_vec3_add(cam->position, forward, target);

  glm_lookat(
      cam->position,
      target,
      (vec3){0.0f, 1.0f, 0.0f},
      cam->view
      );

  //// Projection (update in case aspect changed) ////
  glm_perspective(
      glm_rad(cam->fov),
      aspect_ratio,
      0.1f,
      100.0f,
      cam->projection
      );
}

void camera_get_forward(const Camera *cam, vec3 out) {
  out[0] = cosf(cam->pitch) * sinf(cam->yaw);
  out[1] = sinf(cam->pitch);
  out[2] = cosf(cam->pitch) * cosf(cam->yaw);
}

void camera_get_right(const Camera *cam, vec3 out) {
  out[0] = sinf(cam->yaw - GLM_PI_2);
  out[1] = 0.0f;
  out[2] = cosf(cam->yaw - GLM_PI_2);
}

