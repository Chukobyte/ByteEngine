#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../models/Constants.h"
#include "../InputAction.h"
#include "../Mouse.h"
#include "../Config.h"

class MouseAction {
  public:
    bool isMouseButtonLeftPressed = false;
    bool isMouseButtonLeftJustPressed = false;
    bool isMouseButtonLeftJustPressedLocked = false;
    bool isMouseButtonLeftJustReleased = false;
    bool isMouseButtonRightPressed = false;
    bool isMouseButtonRightJustPressed = false;
    bool isMouseButtonRightJustPressedLocked = false;
    bool isMouseButtonRightJustReleased = false;
};

class InputManager {
  private:
    std::map<std::string, InputAction*> inputActions;
    std::vector<std::string> inputActionReleasedCleanups;
    std::vector<std::string> inputActionJustPressedCleanups;
    MouseAction* mouseAction = new MouseAction();
    std::vector<MouseButtonType> mouseButtonReleasedCleanups;
  public:
    int sdlPollEventCode = -1;
    Mouse mouse;
    InputHandler() {}

    void AddKeyboardInputAction(std::string actionName, std::string actionValue) {
        std::string translatedKeyCode = GetSDLKeyStringCode(actionValue);
        if(inputActions.count(actionName) == 0) {
            InputAction* newInputAction = new InputAction(actionName, translatedKeyCode);
            inputActions.emplace(actionName, newInputAction);
        } else {
            inputActions[actionName]->AddValue(translatedKeyCode);
        }
    }

    bool IsActionPressed(std::string actionName) {
        if(inputActions.count(actionName) <= 0) {
            return false;
        }
        return inputActions[actionName]->isActionPressed;
    }

    bool IsActionJustPressed(std::string actionName) {
        if(inputActions.count(actionName) <= 0) {
            return false;
        }
        return inputActions[actionName]->isActionJustPressed;
    }

    bool IsActionJustReleased(std::string actionName) {
        if(inputActions.count(actionName) <= 0) {
            return false;
        }
        return inputActions[actionName]->isActionJustReleased;
    }

    bool IsMouseButtonLeftPressed() {
        return mouseAction->isMouseButtonLeftPressed;
    }

    bool IsMouseButtonLeftJustPressed() {
        return mouseAction->isMouseButtonLeftJustPressed;
    }

    bool IsMouseButtonLeftJustReleased() {
        return mouseAction->isMouseButtonLeftJustReleased;
    }

    bool IsMouseButtonRightPressed() {
        return mouseAction->isMouseButtonRightPressed;
    }

    bool IsMouseButtonRightJustPressed() {
        return mouseAction->isMouseButtonRightJustPressed;
    }

    bool IsMouseButtonRightJustReleased() {
        return mouseAction->isMouseButtonRightJustReleased;
    }

    void ProcessJustPressedKey(std::string keyCode) {
        for (auto const& mapRef : inputActions) {
            InputAction* inputAction = mapRef.second;
            for(auto& value : inputAction->values) {
                if(value.compare(keyCode) == 0) {
                    inputAction->isActionPressed = true;
                    if(!inputAction->isActionJustPressed && !inputAction->isJustPressedLocked) {
                        inputAction->isActionJustPressed = true;
                        inputAction->isJustPressedLocked = true;
                        inputActionJustPressedCleanups.emplace_back(inputAction->name);
                    }
                    return;
                }
            }
        }
    }

    void ProcessJustReleasedKey(std::string keyCode) {
        for (auto const& mapRef : inputActions) {
            InputAction* inputAction = mapRef.second;
            for(auto& value : inputAction->values) {
                if(value.compare(keyCode) == 0) {
                    inputAction->isActionPressed = false;
                    inputAction->isActionJustPressed = false;
                    inputAction->isJustPressedLocked = false;
                    inputAction->isActionJustReleased = true;
                    inputActionReleasedCleanups.emplace_back(inputAction->name);
                    return;
                }
            }
        }
    }


    void RegisterMousePressed(MouseButtonType mouseButton) {
        if(mouseButton == MOUSE_BUTTON_LEFT) {
            mouseAction->isMouseButtonLeftPressed = true;
            if(mouseAction->isMouseButtonLeftJustPressed == false && mouseAction->isMouseButtonLeftJustPressedLocked == false) {
                mouseAction->isMouseButtonLeftJustPressed = true;
                mouseAction->isMouseButtonLeftJustPressedLocked = true;
            } else if(mouseAction->isMouseButtonLeftJustPressed && mouseAction->isMouseButtonLeftJustPressedLocked) {
                mouseAction->isMouseButtonLeftJustPressed = false;
            }
        } else if(mouseButton == MOUSE_BUTTON_RIGHT) {
            mouseAction->isMouseButtonRightPressed = true;
            if(mouseAction->isMouseButtonRightJustPressed == false && mouseAction->isMouseButtonRightJustPressedLocked == false) {
                mouseAction->isMouseButtonRightJustPressed = true;
                mouseAction->isMouseButtonRightJustPressedLocked = true;
            } else if(mouseAction->isMouseButtonRightJustPressed && mouseAction->isMouseButtonRightJustPressedLocked) {
                mouseAction->isMouseButtonRightJustPressed = false;
            }
        }
    }

    void RegisterMouseReleased(MouseButtonType mouseButton) {
        if(mouseButton == MOUSE_BUTTON_LEFT) {
            mouseAction->isMouseButtonLeftPressed = false;
            mouseAction->isMouseButtonLeftJustReleased = false;
            mouseAction->isMouseButtonLeftJustPressedLocked = false;
            mouseAction->isMouseButtonLeftJustReleased = true;
            mouseButtonReleasedCleanups.emplace_back(MOUSE_BUTTON_LEFT);
        } else if(mouseButton == MOUSE_BUTTON_RIGHT) {
            mouseAction->isMouseButtonRightPressed = false;
            mouseAction->isMouseButtonRightJustReleased = false;
            mouseAction->isMouseButtonRightJustPressedLocked = false;
            mouseAction->isMouseButtonRightJustReleased = true;
            mouseButtonReleasedCleanups.emplace_back(MOUSE_BUTTON_RIGHT);
        }
    }

    void CleanUpInputActions() {
        if(!inputActionReleasedCleanups.empty()) {
            for(std::string actionToCleanName : inputActionReleasedCleanups) {
                inputActions[actionToCleanName]->isActionJustReleased = false;
            }
            inputActionReleasedCleanups.clear();
        }
        if(!mouseButtonReleasedCleanups.empty()) {
            for(MouseButtonType mouseButton : mouseButtonReleasedCleanups) {
                if(mouseButton == MOUSE_BUTTON_LEFT) {
                    mouseAction->isMouseButtonLeftJustReleased = false;
                } else if(mouseButton == MOUSE_BUTTON_RIGHT) {
                    mouseAction->isMouseButtonRightJustReleased = false;
                }
            }
            mouseButtonReleasedCleanups.clear();
        }
        if(!inputActionJustPressedCleanups.empty()) {
            for(std::string actionToCleanName : inputActionJustPressedCleanups) {
                inputActions[actionToCleanName]->isActionJustPressed = false;
            }
            inputActionJustPressedCleanups.clear();
        }
    }

    void ReleaseKeyboardActionsWithoutJustPressedLock() {
        for (auto const& mapRef : inputActions) {
            InputAction* inputAction = mapRef.second;
            inputAction->isActionPressed = false;
            inputAction->isActionJustPressed = false;
            inputAction->isActionJustReleased = false;
        }
    }

    void ReleaseMouseActions() {
        mouseAction->isMouseButtonLeftPressed  = false;
        mouseAction->isMouseButtonLeftJustPressed = false;
        mouseAction->isMouseButtonLeftJustPressedLocked = false;
        mouseAction->isMouseButtonLeftJustReleased = false;
        mouseAction->isMouseButtonRightPressed  = false;
        mouseAction->isMouseButtonRightJustPressed = false;
        mouseAction->isMouseButtonRightJustPressedLocked = false;
        mouseAction->isMouseButtonRightJustReleased = false;
    }

    std::string GetSDLKeyStringCode(std::string key) {
        if(key.compare(KEYBOARD_UP) == 0) {
            return "1073741906";
        }
        if(key.compare(KEYBOARD_DOWN) == 0) {
            return "1073741905";
        }
        if(key.compare(KEYBOARD_RIGHT) == 0) {
            return "1073741903";
        }
        if(key.compare(KEYBOARD_LEFT) == 0) {
            return "1073741904";
        }
        if(key.compare(KEYBOARD_SPACE) == 0) {
            return "32";
        }
        if(key.compare(KEYBOARD_ESC) == 0) {
            return "27";
        }
        // Returns ASCII code
        return std::to_string(static_cast<int>(key[0]));
    }

    void ConfigureInput(std::vector<InputConfig*> inputConfigs) {
        for(InputConfig* config : inputConfigs) {
            if(config->type.compare("keyboard") == 0) {
                AddKeyboardInputAction(config->actionName, config->inputValue);
            }
        }
    }
};

#endif
