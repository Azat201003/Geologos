#version 460

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = vec4(TexCoord.x, TexCoord.y, 1-TexCoord.x * TexCoord.y, texture(ourTexture, TexCoord).r);
}
