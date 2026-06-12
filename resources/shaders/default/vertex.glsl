#version 460

uniform vec4 inColor;
uniform mat4 matrix;
in vec2 pos;
out vec4 color;

void main() {
    color = inColor;
    gl_Position = matrix*vec4(pos.x, pos.y, 0, 1);
}
