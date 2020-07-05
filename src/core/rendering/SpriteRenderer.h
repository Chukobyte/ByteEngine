#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "../../../lib/glm/glm.hpp"
#include "../../../lib/glm/gtc/matrix_transform.hpp"

#include "../managers/ConfigManager.h"

#include "Texture.h"
#include "Shader.h"

class SpriteRenderer {
  private:
    Shader* shader = NULL;
    ConfigManager* configManager = NULL;
    GLuint quadVAO;
    GLuint quadVBO;

  public:
    SpriteRenderer(Shader* shader, ConfigManager* configManager);
    ~SpriteRenderer();
    void Draw(Texture2D* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f));
};

#endif
