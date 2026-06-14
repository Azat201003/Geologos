#pragma once

#include <ft2build.h>
#include <linmath.h>

#include <string>

#include FT_FREETYPE_H
#include <map>
#include <memory>

#include <window/graphics/shaders.h>

struct Character {
	unsigned int texture_id;
	float size_x, size_y;
	float bearing_x, bearing_y;
	unsigned int advance;
};

class Font {
private:
	std::map<char, Character> characters;
public:
	Font(const std::string& path, unsigned size);
	Character get_char(char ch);
	void use(char ch); // texture
};

class TextDrawer {
private:
	unsigned VAO;
	unsigned VBO;

	// after render function
	bool rendered = false;
	std::string text;
	glm::vec4 color;
	std::shared_ptr<Font> font;
public:
	TextDrawer();
	void render(std::shared_ptr<Font> font, float x, float y, std::string text, glm::vec4 color);
	bool draw();
};

enum class FontKit {
	DEFAULT,
};

class FontStorage {
private:
	static std::unordered_map<FontKit, std::shared_ptr<Font>> fonts;
	static const std::unordered_map<FontKit, std::pair<std::string, unsigned>> font_kits;
public:
	static void load();
	static std::shared_ptr<Font> get_font(FontKit);
};

