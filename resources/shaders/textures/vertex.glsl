#version 460

in vec3 aPos;
in vec2 aTexCoord;

uniform mat4 matrix;

out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;
    
    gl_Position = matrix*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
