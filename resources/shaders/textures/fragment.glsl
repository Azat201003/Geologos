#version 460

out vec4 outColor;

in vec2 textureCoord;

uniform sampler2D ourTexture;
uniform vec4 color;

void main() {
    outColor = vec4(color*texture(ourTexture, textureCoord).r);
}
