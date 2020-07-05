#ifndef Helper_H
#define Helper_H

#include <fstream>

#include "../../../lib/json/json.hpp"
#include "../models/Constants.h"

class Helper {
  private:
    template<class T> struct VectorPointerDeleter {
        void operator() (T* x) {
            if(x != NULL) {
                delete x;
            }
        }
    };

  public:
    static ByteConfigKeys hashStringFromConstant(const std::string& text) {
        if(text.compare("game_title") == 0) {
            return eGameTitle;
        }
        if(text.compare("initial_scene") == 0) {
            return eInitialScene;
        }
        if(text.compare("assets") == 0) {
            return eAssets;
        }
        if(text.compare("type") == 0) {
            return eType;
        }
        if(text.compare("id") == 0) {
            return eId;
        }
        if(text.compare("file") == 0) {
            return eFile;
        }
        if(text.compare("font_size") == 0) {
            return eFontSize;
        }
        if(text.compare("animation_frames") == 0) {
            return eAnimationFrames;
        }
        if(text.compare("name") == 0) {
            return eName;
        }
        if(text.compare("texture_id") == 0) {
            return eTextureId;
        }
        if(text.compare("animated") == 0) {
            return eAnimated;
        }
        if(text.compare("animations") == 0) {
            return eAnimations;
        }
        if(text.compare("frames") == 0) {
            return eFrames;
        }
        if(text.compare("speed") == 0) {
            return eSpeed;
        }
        if(text.compare("inputs") == 0) {
            return eInputs;
        }
        if(text.compare("actions") == 0) {
            return eActions;
        }
        if(text.compare("input_values") == 0) {
            return eInputValues;
        }
        if(text.compare("index") == 0) {
            return eIndex;
        }
        if(text.compare("layer") == 0) {
            return eLayer;
        }
        if(text.compare("entities") == 0) {
            return eEntities;
        }
        if(text.compare("components") == 0) {
            return eComponents;
        }
        if(text.compare("module") == 0) {
            return eModule;
        }
        if(text.compare("class") == 0) {
            return eClass;
        }
        if(text.compare("window") == 0) {
            return eWindow;
        }
        if(text.compare("width") == 0) {
            return eWidth;
        }
        if(text.compare("height") == 0) {
            return eHeight;
        }
        if(text.compare("position") == 0) {
            return ePosition;
        }
        if(text.compare("velocity") == 0) {
            return eVelocity;
        }
        if(text.compare("scale") == 0) {
            return eScale;
        }
        if(text.compare("rotation") == 0) {
            return eRotation;
        }
        if(text.compare("camera") == 0) {
            return eCamera;
        }
        if(text.compare("camera_focused") == 0) {
            return eCameraFocused;
        }
        if(text.compare("zoom") == 0) {
            return eZoom;
        }
        if(text.compare("collider_tag") == 0) {
            return eColliderTag;
        }
        if(text.compare("size") == 0) {
            return eSize;
        }
        if(text.compare("offset") == 0) {
            return eOffset;
        }
        if(text.compare("background_color") == 0) {
            return eBackgroundColor;
        }
        if(text.compare("color") == 0) {
            return eColor;
        }
        if(text.compare("text") == 0) {
            return eText;
        }
        if(text.compare("font_id") == 0) {
            return eFontId;
        }
        if(text.compare("wrap_length") == 0) {
            return eWrapLength;
        }
        if(text.compare("default_script_class") == 0) {
            return eDefaultScriptClass;
        }
        if(text.compare("external_scene_reference") == 0) {
            return eExternalSceneReference;
        }
        return eKeyValueNotFound;
    }

    static nlohmann::json LoadJsonFile(std::string filePath) {
        std::ifstream i(filePath);
        nlohmann::json json;
        i >> json;
        return json;
    }

    static float ConvertMinMax(float input, float inputLow, float inputHigh, float outputLow, float outputHigh) {
        return (((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow);
    }

    template<class T> static void ClearVectorOfPointer(std::vector<T*> &itemList) {
        for_each(itemList.begin(), itemList.end(), VectorPointerDeleter<T>());
        itemList.clear();
    };
};

#endif
