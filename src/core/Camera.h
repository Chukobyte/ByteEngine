#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"

class Camera {
  public:
    SDL_Rect viewport;
    glm::uvec2 zoom;
};

#endif
