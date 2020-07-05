#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../../lib/glm/glm.hpp"
#include "../../../lib/glm/gtc/matrix_transform.hpp"

#include "../helpers/Helper.h"
#include "../managers/ConfigManager.h"

#include "./Font.h"

class TextRenderer {
  private:
    Shader* shader = NULL;
    ConfigManager* configManager = NULL;
  public:
    FT_Library freeTypeLibrary;

    TextRenderer(Shader* shader, ConfigManager* configManager) {
        this->shader = shader;
        this->configManager = configManager;
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(configManager->windowWidth), 0.0f, static_cast<float>(configManager->windowHeight));
        this->shader->Use();
        this->shader->SetMatrix4Float("projection", projection);

        if(FT_Init_FreeType(&freeTypeLibrary)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
    }

    ~TextRenderer() {
        FT_Done_FreeType(freeTypeLibrary);
    }

    void Draw(Font* font, std::string text, float x, float y, float scale, glm::vec3 color) {
        // activate corresponding font render state
        shader->Use();
        shader->SetVec3Float("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(font->VAO);

        // Converts OpenGL y coords to game engine's
        y = Helper::ConvertMinMax(y, static_cast<float>(configManager->windowHeight), 0, 0, static_cast<float>(configManager->windowHeight));

        // iterate through all characters
        std::string::const_iterator c;
        for(c = text.begin(); c != text.end(); c++) {
            Character ch = font->characters[*c];

            float xPos = x + ch.bearing.x * scale;
            float yPos = y - ch.size.y * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;
            // update VBO for each character
            GLfloat vertices[6][4] = {
                {xPos,     yPos + h, 0.0f, 0.0f},
                {xPos,     yPos,     0.0f, 1.0f},
                {xPos + w, yPos,     1.0f, 1.0f},

                {xPos,     yPos + h, 0.0f, 0.0f},
                {xPos + w, yPos,     1.0f, 1.0f},
                {xPos + w, yPos + h, 1.0f, 0.0f}
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // advance cursor for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

};

#endif
