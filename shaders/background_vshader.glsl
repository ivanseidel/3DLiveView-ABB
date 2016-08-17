#version 410

in vec4 vPosition;
in vec2 vcoordText;

out vec2 ftexCoord;

void main() {
  gl_Position = vPosition;
  ftexCoord = vcoordText;
}
