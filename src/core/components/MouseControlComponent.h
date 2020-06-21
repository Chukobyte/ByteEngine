#ifndef MOUSECONTROLCOMPONENT_H
#define MOUSECONTROLCOMPONENT_H

#include <SDL2/SDL.h>

#include "../../../lib/glm/glm.hpp"
#include "../models/Constants.h"
#include "../Game.h"

class MouseControlComponent : public Component {
  private:
    InputManager* inputManager;

  public:
    MouseControlComponent(InputManager* inputManager) {
        this->inputManager = inputManager;
    }

    void Initialize() override {}

    void Update(float deltaTime) override {
        if(inputManager->sdlPollEventCode != 1) {
            inputManager->ReleaseMouseActions();
            return;
        }
        // Coupled with InputManager's mouse for now
        if(Game::event.type == SDL_MOUSEMOTION) {
            inputManager->mouse.motion.x = Game::event.motion.x;
            inputManager->mouse.motion.y = Game::event.motion.y;
        }
        if(Game::event.type == SDL_MOUSEBUTTONDOWN) {
            switch (Game::event.button.button) {
            case SDL_BUTTON_LEFT:
                inputManager->RegisterMousePressed(MOUSE_BUTTON_LEFT);
                break;
            case SDL_BUTTON_RIGHT:
                inputManager->RegisterMousePressed(MOUSE_BUTTON_RIGHT);
                break;
            default:
                break;
            }
        } else if(Game::event.type == SDL_MOUSEBUTTONUP) {
            switch (Game::event.button.button) {
            case SDL_BUTTON_LEFT:
                inputManager->RegisterMouseReleased(MOUSE_BUTTON_LEFT);
                break;
            case SDL_BUTTON_RIGHT:
                inputManager->RegisterMouseReleased(MOUSE_BUTTON_RIGHT);
                break;
            default:
                break;
            }
        } else {
            // Free all things if no event is sent
            inputManager->ReleaseMouseActions();
        }
    }
};

#endif
