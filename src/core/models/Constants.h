#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Project Settings
const std::string VERSION = "0.0.2";
const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;
const float MILLISECONDS_PER_TICK = 1000.0f;
const float MAX_DELTA_TIME = 0.05f;

enum ByteConfigKeys {
	eGameTitle,
	eWindow,
	eWidth,
	eHeight,
	eInitialScene,
	eAssets,
	eAnimationFrames,
	eInputs,
	eType,
	eId,
	eFile,
	eFontSize,
	eName,
	eTextureId,
	eAnimations,
	eFrames,
	eSpeed,
	eActions,
	eInputValues,
	eIndex,
	eLayer,
	eExternalSceneReference,
	eIsClickable,
	eCamera,
	eCameraFocused,
	eEntities,
	eComponents,
	eModule,
	eClass,
	eAnimated,
	ePosition,
	eVelocity,
	eRotation,
	eScale,
	eZoom,
	eColliderTag,
	eSize,
	eOffset,
	eBackgroundColor,
	eColor,
	eText,
	eFontId,
	eWrapLength,
	eDefaultScriptClass,
	eKeyValueNotFound
};

// Keyboard Keys
const std::string KEYBOARD_UP = "up";
const std::string KEYBOARD_DOWN = "down";
const std::string KEYBOARD_RIGHT = "right";
const std::string KEYBOARD_LEFT = "left";
const std::string KEYBOARD_SPACE = "space";
const std::string KEYBOARD_ESC = "esc";

// Mouse
enum MouseButtonType {
    MOUSE_BUTTON_LEFT = 0,
    MOUSE_BUTTON_RIGHT = 1,
    KEY_PRESSED = 2,
    KEY_RELEASED = 3
};

#endif
