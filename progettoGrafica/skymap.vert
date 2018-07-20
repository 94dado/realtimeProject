#version 330 core

in vec3 vp;
uniform mat4 P, V;  //projection, view
out vec3 texcoords;

void main() {
  texcoords = vp;
  gl_Position = P * V * vec4(vp, 1.0);
}