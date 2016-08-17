#version 410
in vec4 inoutColor;
in vec2 ftexCoord;
uniform sampler2D colorTexture;
uniform float textureOpacity;
out vec4 myfragcolor;

void main () {
  vec4 imgColor = texture(colorTexture, ftexCoord);
  myfragcolor = inoutColor + imgColor * textureOpacity;
}
