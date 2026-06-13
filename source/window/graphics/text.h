#pragma once

#include <ft2build.h>
#include <linmath.h>

#include <string>

#include FT_FREETYPE_H
#include <map>

#include <window/graphics/shaders.h>

struct Character {
	unsigned int texture_id;
	float size_x, size_y;
	float bearing_x, bearing_y;
	unsigned int advance;
};

class TextDrawer {
private:
	std::map<char, Character> characters;
	Shader* shader = nullptr;
	unsigned VAO;
	unsigned VBO;
public:
	void render(float x, float y, std::string text, glm::vec4 color); // should be called only in opengl render thread
	void draw(); // should be called only in opengl render thread
	TextDrawer(std::string fontpath, unsigned height);
};
