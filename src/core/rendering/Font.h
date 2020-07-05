#ifndef FONT_H
#define FONT_H

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include <glad/glad.h>

#include "../../../lib/glm/glm.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;

};

class Font {
  private:
    bool DoesFileExist(const std::string& name) {
        struct stat buffer;
        return(stat(name.c_str(), &buffer) == 0);
    }
  public:
    std::map<GLchar, Character> characters;
    GLuint VAO;
    GLuint VBO;

    Font(FT_Library freeTypeLibrary, const char* fileName, int size) {
        LoadFont(freeTypeLibrary, fileName, size);
        // configure VAO & VBO texture quads
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    LoadFont(FT_Library freeTypeLibrary, const char* fileName, int size) {
        if(!DoesFileExist(fileName)) {
            std::cout << "ERROR::FREETYPE: Failed to find font at path" << std::endl;
        }
        FT_Face face;
        if(FT_New_Face(freeTypeLibrary, fileName, 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        } else {
            // set size to load glyphs. width set to 0 to dynamically adjust
            FT_Set_Pixel_Sizes(face, 0, size);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for(unsigned char c = 0; c < 128; c++) {
                // load character glyph
                if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                    std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int textTexture;
                glGenTextures(1, &textTexture);
                glBindTexture(GL_TEXTURE_2D, textTexture);
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
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // store character for later use
                Character character = {
                    textTexture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };
                characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        FT_Done_Face(face);
    }

    // TODO: add proper validation function
    bool IsValid() {
        return true;
    }
};

#endif
