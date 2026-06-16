#version 460

out vec4 outColor;

in vec2 textureCoord;

uniform sampler2D ourTexture;
uniform vec4 color;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(ourTexture, textureCoord).r);
	outColor = color * sampled;

	//outColor = vec4(color.r, color.g, color.b, color.a*texture(ourTexture, textureCoord).r);
}

