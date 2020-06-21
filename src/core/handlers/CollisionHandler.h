#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <SDL2/SDL.h>

class CollisionHandler {
  public:
  	static bool CheckRectangleCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB) {
	    return CheckRectangleCollision(rectangleA, rectangleB, glm::vec2(0, 0));
	}
    static bool CheckRectangleCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB, glm::vec2 offset) {
	    return (
	               rectangleA.x + offset.x + rectangleA.w >= rectangleB.x &&
	               rectangleB.x + offset.x + rectangleB.w >= rectangleA.x &&
	               rectangleA.y + offset.y + rectangleA.h >= rectangleB.y &&
	               rectangleB.y + offset.y + rectangleB.h >= rectangleA.y
	    );
	}
};

#endif
