#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>

class Animation {
  public:
    std::string name;
    unsigned int index;
    unsigned int numFrames;
    unsigned int animationSpeed;
    Animation() {}
    Animation(std::string name, unsigned int index, unsigned int numFrames, unsigned int animationSpeed) {
    	this->name = name;
    	this->index = index;
    	this->numFrames = numFrames;
    	if(animationSpeed <= 0) {
	        animationSpeed = 1;
	    }
	    this->animationSpeed = animationSpeed;
   }
};

#endif
