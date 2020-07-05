#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Project Settings
static const std::string VERSION = "0.0.2";
static const unsigned int FPS = 60;
static const unsigned int FRAME_TARGET_TIME = 1000 / FPS;
static const float MILLISECONDS_PER_TICK = 1000.0f;
static const float MAX_DELTA_TIME = 0.05f;
static const char* DEFAULT_SPRITE_VERTEX_SHADER_PATH = "./src/core/rendering/shaders/sprite_default.vshader";
static const char* DEFAULT_SPRITE_FRAGMENT_SHADER_PATH = "./src/core/rendering/shaders/sprite_default.fshader";
static const char* DEFAULT_TEXT_VERTEX_SHADER_PATH = "./src/core/rendering/shaders/text_default.vshader";
static const char* DEFAULT_TEXT_FRAGMENT_SHADER_PATH = "./src/core/rendering/shaders/text_default.fshader";
static const float RGB_WHOLE_NUMBER_COLOR_CONVERSION = 0.003921568627451f;

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
