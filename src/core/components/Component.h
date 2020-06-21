#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
  public:
    Entity* owner;
    Component() {}
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    //virtual void UpdateTransform(TransformComponent* newTransform) {}
};

#endif
