#version 410
#define PI 3.1415926535897932384626433832795

in vec2 ftexCoord;
uniform mat4 view;
uniform sampler2D colorTexture;
out vec4 outColor;

void main(void) {

  // Gets the Direction of Flight (DOF) from the view matrix
  vec3 dir = vec3(view[0][2], view[1][2], view[2][2]);

  float rotation = atan(dir.x, dir.z);
  float tilt = atan(dir.x, dir.y);

  vec2 texSize = textureSize(colorTexture, 0);
  float aspect = texSize.x / texSize.y;
//  vec2 d = 1.0 / texSize;

  // Gets the texture sample at position
  vec2 cord = vec2(ftexCoord.x, ftexCoord.y);
  vec4 imgColor = texture(colorTexture, vec2(cord.x / aspect, cord.y) + vec2(-rotation / 2.0 / PI * aspect, tilt / 2.0 / PI));

  outColor = imgColor;
}
