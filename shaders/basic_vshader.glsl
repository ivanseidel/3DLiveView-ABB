# version 410
in vec4 vPosition;
in vec3 vNormal;
uniform mat4 model;
out vec4 v2fcolor;

void main()
{
    gl_Position = model * vPosition;
    //gl_Position = vPosition;
    float gray = (vPosition.z + 1) * 0.5;
    float x = abs(vNormal.x);
    float y = abs(vNormal.y);
    float z = abs(vNormal.z);
    v2fcolor = vec4(x, y, z, 1.0);
}
