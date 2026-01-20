#version 330 core

// interpolated values from vertex shader
in vec3 fragmentColor;

out vec3 color;

void main() {
  color = fragmentColor;
}
