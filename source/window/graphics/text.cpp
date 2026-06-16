#include <freetype/config/ftheader.h>
#include <freetype/freetype.h>
#include <window/graphics/matrix.h>
#include <window/graphics/text.h>

#include "window/graphics/shaders.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

FT_Library ft;

Font::Font(const std::string& fontpath, unsigned size) {
	this->size = size;

	auto err = FT_Init_FreeType(&ft);
	if (err) {
		spdlog::error("FreeType library wasn't successfully loaded");
	}

	FT_Face face;
	err = FT_New_Face(ft, fontpath.data(), 0, &face);
	if (err) {
		spdlog::error("FreeType face wasn't successfully created");
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	for (unsigned char c = 0; c < 128; c++) {
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_DEFAULT)) {
			continue;
		}

		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		
		// Generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		Character character = {
			texture,
			(float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows,
			(float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top,
			(unsigned int)face->glyph->advance.x
		};
		characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	spdlog::info("Font loaded ({}, {}px)", fontpath, size);
}

void Font::use(char ch) {
	glBindTexture(GL_TEXTURE_2D, characters[ch].texture_id);
}

Character Font::get_char(char ch) {
	return characters[ch];
}

unsigned Font::get_size() {
	return this->size;
}

TextDrawer::TextDrawer() {	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void TextDrawer::char_vertices(Character character, float x, float y, float vertices[6][5]) {
	float xpos = x + character.off_x;
	float ypos = y + font->get_size() - character.off_y;

	float w = character.size_x;
	float h = character.size_y;

	vertices[0][0] = xpos;
	vertices[0][1] = ypos;
	vertices[0][2] = 0.f;
	vertices[0][3] = 0.f;
	vertices[0][4] = 0.f;

	vertices[1][0] = xpos;
	vertices[1][1] = ypos + h;
	vertices[1][2] = 0.f;
	vertices[1][3] = 0.f;
	vertices[1][4] = 1.f;
	
	vertices[2][0] = xpos + w;
	vertices[2][1] = ypos + h;
	vertices[2][2] = 0.f;
	vertices[2][3] = 1.f;
	vertices[2][4] = 1.f;
	

	vertices[3][0] = xpos;
	vertices[3][1] = ypos;
	vertices[3][2] = 0.f;
	vertices[3][3] = 0.f;
	vertices[3][4] = 0.f;
	
	vertices[4][0] = xpos + w;
	vertices[4][1] = ypos + h;
	vertices[4][2] = 0.f;
	vertices[4][3] = 1.f;
	vertices[4][4] = 1.f;
	
	vertices[5][0] = xpos + w;
	vertices[5][1] = ypos;
	vertices[5][2] = 0.f;
	vertices[5][3] = 1.f;
	vertices[5][4] = 0.f;
}

void TextDrawer::render(const TextDrawer::RenderParams& params) {
	rendered = true;
	this->text = params.text;
	this->color = params.color;
	this->font = params.font;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	float vertices[text.size()*6][5];
	float x = params.x;
	float y = params.y;
	float last_x = x;
	unsigned fontsize = font->get_size();

	int i = 0;
	int last_i = 0;
	for (const char& c : text) {
		Character character = font->get_char(c);
		if (c == ' ') {
			if (x - params.x > params.width) {
				last_x = params.x;
				y += fontsize;
			}
			x = last_x;
			for (int j = last_i; j <= i; ++j) {
				Character character = font->get_char(text[j]);
				char_vertices(character, x, y, &vertices[6*j]);
				x += (character.advance >> 6);
			}
			last_x = x;
			last_i = i+1;
		}
		i++;
		x += (character.advance >> 6);
	}

	if (x - params.x > params.width) {
		last_x = params.x;
		y += fontsize;
	}
			
	x = last_x;

	for (int j = last_i; j < i; ++j) {
		Character character = font->get_char(text[j]);
		char_vertices(character, x, y, &vertices[6*j]);
		x += (character.advance >> 6);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool TextDrawer::draw() {
	if (!rendered) {
		spdlog::warn("Try to draw text, that wasn't rendered");
		return false;
	}
	glEnable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glActiveTexture(GL_TEXTURE0);

	auto shader = ShaderStorage::get_shader(ShaderKit::TEXTURES);
	shader->use();
	shader->set_int("ourTexture", 0);
	shader->set_mat4("matrix",	matrix);
	shader->set_vec4("color",  color);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int i = 0;
	for (const char& c : text) {
		font->use(c);
		glDrawArrays(GL_TRIANGLES, 6*i, 6);
		i++;
	}
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	return true;
}

const std::unordered_map<FontKit, std::pair<std::string, unsigned>> FontStorage::font_kits {
	{FontKit::REGULAR_PARAGRAPH, {"resources/fonts/Inter_18pt-Regular.ttf", 18}}
};

std::unordered_map<FontKit, std::shared_ptr<Font>> FontStorage::fonts {};

void FontStorage::load() {
	for (auto font_kit : font_kits) {
		fonts.emplace(font_kit.first, std::make_shared<Font>(font_kit.second.first, font_kit.second.second));
	}
	spdlog::info("Fonts loaded");
}

std::shared_ptr<Font> FontStorage::get_font(FontKit font_kit) {
	return fonts[font_kit];
}

