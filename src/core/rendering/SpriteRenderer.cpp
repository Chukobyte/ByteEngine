#include "SpriteRenderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader* shader, ConfigManager* configManager) {
    this->shader = shader;
    this->configManager = configManager;
    // Setup vertex data (and buffer(s)) and configure vertex attributes
    GLfloat vertices[] = {
        // positions // texture coordinates
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    // OpenGL State
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Initialize render data
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // texture coordinate attribute
    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set shader stuff
    shader->Use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(configManager->windowWidth), static_cast<float>(configManager->windowHeight), 0.0f, -1.0f, 1.0f);
    shader->SetMatrix4Float("projection", projection);
    shader->SetInt("sprite", 0);
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void SpriteRenderer::Draw(Texture2D* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, float rotation, glm::vec3 color) {
    glDepthMask(false);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(destinationRectangle.x, destinationRectangle.y, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * destinationRectangle.w, 0.5f * destinationRectangle.h, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * destinationRectangle.w, -0.5f * destinationRectangle.h, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(destinationRectangle.w, destinationRectangle.h, 1.0f)); // last scale

    shader->Use();
    shader->SetMatrix4Float("model", model);
    shader->SetVec3Float("spriteColor", color);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    // render subimage based on source rectangle
    glPixelStorei(GL_UNPACK_ROW_LENGTH, texture->width);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, sourceRectangle.x);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, sourceRectangle.y);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sourceRectangle.w, sourceRectangle.h, 0, texture->imageFormat, GL_UNSIGNED_BYTE, texture->data);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0 );

    // render container
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glDepthMask(true);
}
