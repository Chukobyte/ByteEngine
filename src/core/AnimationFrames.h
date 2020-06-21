#ifndef ANIMATIONFRAMES_H
#define ANIMATIONFRAMES_H

#include <map>

#include "./Animation.h"
#include "./Config.h"

class AnimationFrames {
  private:
    std::string NO_VALUE = "[!__NO_VALUE__!]";
    std::map<std::string, Animation> animations;
    bool activated;
  public:
    std::string name;
    std::string textureId;
    std::string firstAnimationName = NO_VALUE;

    AnimationFrames() {
        this->name = NO_VALUE;
        this->textureId = NO_VALUE;
        this->activated = false;
    }

    AnimationFrames(std::string name, std::string textureId) {
        this->name = name;
        this->textureId = textureId;
        this->activated = true;        
    }

    void AddAnimation(Animation animation) {
        if(this->firstAnimationName.compare(NO_VALUE) == 0) {
            this->firstAnimationName = animation.name;
        }
        animations.emplace(animation.name, animation);
    }

    Animation GetAnimation(std::string animationName) {
        return animations[animationName];
    }

    std::string GetFirstAnimationName() {
        return firstAnimationName;
    }
};

#endif
