#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "./Component.h"
#include "../../../lib/glm/glm.hpp"

class TransformComponent: public Component {
  public:
    glm::vec2 position;
    glm::vec2 velocity;
    int width;
    int height;
    float scale;

    TransformComponent(int posX, int posY, int velX, int velY, int w, int h, float s) {
        position = glm::vec2(posX, posY);
        velocity = glm::vec2(velX, velY);
        width = w;
        height = h;
        scale = s;
    }

    void Initialize() override {}

    void Update(float deltaTime) override {}

    void Render() override {}
};

#endif
