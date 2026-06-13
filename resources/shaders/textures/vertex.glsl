#version 460

in vec3 aPos;
in vec2 aTextureCoord;

uniform mat4 matrix;

out vec2 textureCoord;

void main() {
    textureCoord = aTextureCoord; 
    gl_Position = matrix*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
