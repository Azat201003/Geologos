#include <window/graphics/text.h>
#include <window/graphics/matrix.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// #include <iostream>

FT_Library ft;

TextDrawer::TextDrawer(std::string fontpath, unsigned height) {
    auto err = FT_Init_FreeType(&ft);
    if (err) {
        throw err; 
    }

    FT_Face face;
    err = FT_New_Face(ft, fontpath.data(), 0, &face);
    if (err) {
        throw err;
    }

    FT_Set_Pixel_Sizes(face, 0, height);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }
        
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
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
    
    shader = new Shader("resources/shaders/textures/vertex.glsl", "resources/shaders/textures/fragment.glsl");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void TextDrawer::render(float x, float y, std::string text, vec3 color) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glActiveTexture(GL_TEXTURE0);

    shader->use();
    shader->set_int("ourTexture", 0);
    shader->set_mat4("matrix",  matrix);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (const char c : text) {
        Character character = characters[c];

        float xpos = x + character.bearing_x;
        float ypos = y - character.size_y;

        float w = character.size_x;
        float h = character.size_y;
        
        float vertices[6][5] = {
            { xpos,     ypos,   0.f,   0.f, 0.f },            
            { xpos,     ypos + h,       0.f,   0.f, 1.f },
            { xpos + w, ypos + h,       0.f,   1.f, 1.f },

            { xpos,     ypos,   0.f,   0.f, 0.f },
            { xpos + w, ypos + h,       0.f,   1.f, 1.f },
            { xpos + w, ypos,   0.f,   1.f, 0.f }           
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glBindTexture(GL_TEXTURE_2D, character.texture_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        x += (character.advance >> 6);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
