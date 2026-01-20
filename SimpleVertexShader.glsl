
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;

// output data. interpolated for each fragment
out vec3 fragmentColor;

// main gets called for each vertex 
void main() {

  gl_Position = MVP * vec4(vertexPosition_modelspace,1);

  // gl_Position.xyz = vertexPosition_modelspace;
  // gl_Position.w = 1.0;
  
  // the color of each vertex will be interpolated to produce 
  // the color for each fragment
  fragmentColor = vertexColor;

} 
