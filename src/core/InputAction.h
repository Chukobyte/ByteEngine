#ifndef INPUTACTION_H
#define INPUTACTION_H

class InputAction {
  public:
    std::string name;
    std::vector<std::string> values;
    bool isActionPressed = false;
    bool isJustPressedLocked = false;
    bool isActionJustPressed = false;
    bool isActionJustReleased = false;

    InputAction() {}

    InputAction(std::string actionName, std::string actionValue) {
        this->name = actionName;
        AddValue(actionValue);
    }

    void AddValue(std::string valueKey) {
        values.emplace_back(valueKey);
    }
};

#endif
