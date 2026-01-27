#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "./include/shader_c.h"
#include "./include/cglm/cglm.h"

#include "./include/fast_obj-1.3/fast_obj.h"
#include "./include/fast_obj-1.3/fast_obj.c"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define SCREEN_RATIO_X 4
#define SCREEN_RATIO_Y 3

int main() {

  srand(time(NULL));

  /// init GLFW ///
  glewExperimental = true; // Needed for core profile
  // libdecor bug would cause constant window resize on hypland
  // glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
  if (!glfwInit()) {
    fprintf(stderr, "glfw init failed\n");
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // open a window and create its openGL context
  GLFWwindow *window; // pointer to window
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "openGL", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "failed to open GLFW window\n");
    glfwTerminate();
    return -1;
  }

  // init glew
  glfwMakeContextCurrent(window); 
  glewExperimental = true;
  GLenum glew_status = glewInit();
  // fuck this. glew wayland bug
  if (glew_status != GLEW_OK && glew_status != 4) {
    glewGetErrorString(glewInit());
    fprintf(stderr, "glew init failed\n");
    return -1;
  }

  // capture the escape key being pressed
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Create VAO (vertex array object)
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // 3D model loading
  fastObjMesh *mesh = fast_obj_read("./assets/120KRH92.obj");

  // GLfloat g_vertex_buffer_data[sizeof(mesh->positions[3]) * (mesh->position_count * 3)];

  // Count total vertices..... What the fuck??
  size_t vertex_count = 0;
  for (unsigned int i = 0; i < mesh->face_count; i++) {
    vertex_count += mesh->face_vertices[i];
  }

  GLfloat *vertices = malloc(vertex_count * 3 * sizeof(GLfloat));

  size_t vertex_floats_count = 0;
  size_t index_offset = 0;

  for (unsigned int f = 0; f < mesh->face_count; f++) {
    for (unsigned int fv = 0; fv < mesh->face_vertices[f]; fv++) {
      fastObjIndex idx = mesh->indices[index_offset + fv];

      vertices[vertex_floats_count++] = mesh->positions[idx.p * 3 + 0];
      vertices[vertex_floats_count++] = mesh->positions[idx.p * 3 + 1];
      vertices[vertex_floats_count++] = mesh->positions[idx.p * 3 + 2];
    }
    index_offset += mesh->face_vertices[f];
  }

  fast_obj_destroy(mesh);

  // this will identify vertex buffer
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vetexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

  // Give verticies to OpenGL
  glBufferData(
      GL_ARRAY_BUFFER, 
      vertex_floats_count * sizeof(GLfloat), 
      vertices, 
      GL_STATIC_DRAW
      );

  // color buffer
  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  // glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  // this (paragraph) needs to be done only once ^

  /// MVP MATRIX. TO BE DONE FOR EACH MODEL RENDERED(?) ///
  mat4 projection;
  mat4 view;
  mat4 mvp;

  // create and compile glsl program from the shaders
  GLuint programID = LoadShaders_C( "SimpleVertexShader.glsl", "SimpleFragmentShader.glsl" );
  
  /// Give MVP to GLSL ///
  GLuint matrixID = glGetUniformLocation(programID, "MVP");
  
  // mouse
  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    printf("no raw mouse support\n");
    return 1;
  }

  float mouse_speed = 0.6f;

  vec3 position = {0.0f, 0.0f, 5.0f};
  float horizontal_angle = 3.14f;
  float vertical_angle = 0.0f;
  const float vertical_angle_restriction = 1.5f;
  float fov = 90.0f;

  /////// MAIN LOOP ///////
  double current_time = glfwGetTime();
  do {

    double last_time = current_time;
    current_time = glfwGetTime();
    float delta_time = (float) current_time - last_time;
    // printf("%f\n", delta_time);

    float speed = 3.0f;

    // mouse pos
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // printf("%f %f\n", xpos, ypos);
    glfwSetCursorPos(window, 0, 0);

    // camera rotation
    // TODO: fix possible buffer overflow on angles;
    horizontal_angle += mouse_speed * delta_time * -xpos;
    vertical_angle += mouse_speed * delta_time * -ypos;
    if (vertical_angle > vertical_angle_restriction) {
      vertical_angle = vertical_angle_restriction;
    } else if (vertical_angle < -vertical_angle_restriction) {
      vertical_angle = -vertical_angle_restriction;
    } 
    // printf("horizontal angle: %f\n", horizontal_angle);
    // printf("vertical angle: %f\n", vertical_angle);

    vec3 direction = {
      cos(vertical_angle) * sin(horizontal_angle),
      sin(vertical_angle),
      cos(vertical_angle) * cos(horizontal_angle)
    };
    vec3 right = {
      sin(horizontal_angle - 3.14f/2.0f),
      0,
      cos(horizontal_angle -3.14f/2.0f)
    };
    vec3 up;
    glm_cross(right, direction, up);

    if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT )) {
        speed = 6.0f;
    }

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
      vec3 delta;
      glm_vec3_scale(direction, delta_time * speed, delta);
      glm_vec3_add(position, delta, position);
    }

    // Move backward
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
      vec3 delta;
      glm_vec3_scale(direction, delta_time * -speed, delta);
      glm_vec3_add(position, delta, position);
    }

    // Move left
    if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
      vec3 delta;
      glm_vec3_scale(right, delta_time * -speed, delta);
      glm_vec3_add(position, delta, position);
    }

    // Move right
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
      vec3 delta;
      glm_vec3_scale(right, delta_time * (1 * speed), delta);
      glm_vec3_add(position, delta, position);
    }

    // random colors per vertex 
    GLfloat *color_data = malloc(vertex_floats_count * sizeof(GLfloat)); // same float count as vertices
    for (size_t i = 0; i < vertex_floats_count / 3; i++) { // one color per vertex
      float r = (float)rand() / RAND_MAX;
      float g = (float)rand() / RAND_MAX;
      float b = (float)rand() / RAND_MAX;
      color_data[i*3 + 0] = r;
      color_data[i*3 + 1] = g;
      color_data[i*3 + 2] = b;
    }

    ////// MVP //////

    glm_perspective(glm_rad(fov), 
        (float) SCREEN_RATIO_X / (float) SCREEN_RATIO_Y,
        0.1f, 100.0f, projection
        );
    vec3 view_direction;
    glm_vec3_add(position, direction, view_direction),
    glm_lookat(
        position,
        view_direction,
        (vec3) {0.0f,1.0f,0.0f}, // Head is up
        view // destination (output)
        );
    mat4 model;
    glm_mat4_identity(model);
    glm_mat4_mulN((mat4 *[]){&projection, &view, &model}, 3, mvp);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    // 1st attribute buffer: verticies
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0, // attribute 0 ????
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

    // send transformation to the currently bound shader, in the "MVP" uniform
    // this is done in the main loop since each model will have differnet MVP matrix
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    // 2nd attribute buffer: colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_floats_count * sizeof(GLfloat), color_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
        1, // attribute 1 ????
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

    // Enable depth test && backface culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // Accept fragment if its closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Draw something 
    glDrawArrays(GL_TRIANGLES, 0, vertex_floats_count / 3); // 0 -> N triangles
    glDisableVertexAttribArray(0);
      
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the Q key was pressed or the window was closed
  while(
      glfwGetKey(window, GLFW_KEY_Q ) != GLFW_PRESS &&
      glfwWindowShouldClose(window) == 0 
      );

  printf("done\n");
  return 0;
}
