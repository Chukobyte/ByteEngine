#ifndef ANIMATIONFRAMEMANAGER_H
#define ANIMATIONFRAMEMANAGER_H

#include <map>
#include <string>

#include "../../../lib/json/json.hpp"
#include "../AnimationFrames.h"

class AnimationFrameManager {
	private:
		std::map<std::string, AnimationFrames> animationFramesRepository;
	public:
		void AddAnimationFrames(std::string textureId, AnimationFrames animationFrames) {
			animationFramesRepository.emplace(textureId, animationFrames);
		}

    	bool HasAnimationFrames(std::string textureId) {
    		return animationFramesRepository.count(textureId) > 0;
    	}

    	AnimationFrames GetAnimationFrames(std::string textureId) {
    		return animationFramesRepository[textureId];
    	}

    	void SetAnimationFrames(std::map<std::string, AnimationFrames> animationFrames) {
    		this->animationFramesRepository = animationFrames;
    	}
};

#endif
