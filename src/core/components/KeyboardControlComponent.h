#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <iostream>
#include <string>
#include "../Game.h"
#include "../managers/InputManager.h"

class KeyboardControlComponent : public Component {
  private:
    InputManager* inputManager;
  public:
    KeyboardControlComponent(InputManager* inputManager) {
        this->inputManager = inputManager;
    }

    void Initialize() override {}

    void Update(float deltaTime) override {
        if(inputManager->sdlPollEventCode != 1) {
            return;
        }
        if(Game::event.type == SDL_KEYDOWN) {
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            inputManager->ProcessJustPressedKey(keyCode);
        } else if(Game::event.type == SDL_KEYUP && Game::event.key.repeat == 0) {
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            inputManager->ProcessJustReleasedKey(keyCode);
        } else {
            // Free all things if no event is sent
            inputManager->ReleaseKeyboardActionsWithoutJustPressedLock();
        }
    }
};


#endif
