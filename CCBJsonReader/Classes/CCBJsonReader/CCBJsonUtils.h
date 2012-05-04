//
//  CCBJsonUtils.h
//  CCBJsonExample
//
//  Created by 靖宇 张 on 12-4-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CCBJsonExample_CCBJsonUtils_h
#define CCBJsonExample_CCBJsonUtils_h

#include "cocos2d.h"
#include "json/json.h"

class CCBJsonUtils {
public:
    struct ScaleLockType {
        float scaleX;
        float scaleY;
        bool unknown;
    };
    
public:
    static bool valueFromFile(const char* file,Json::Value& value);
    static cocos2d::CCPoint pointFromValue(const Json::Value& value);
    static cocos2d::CCSize sizeFromValue(const Json::Value& value);
    static ScaleLockType scaleFromValue(const Json::Value& value);
    static std::pair<std::string,std::string> displayFrameFromValue(const Json::Value& value);
    static cocos2d::ccColor3B color3bFromValue(const Json::Value& value);
    static std::pair<bool,bool> flipFromValue(const Json::Value& value);
    static cocos2d::ccBlendFunc blendFuncFromValue(const Json::Value& value);
    static std::pair<float,float> floatVarFromValue(const Json::Value& value);
    static std::pair<cocos2d::ccColor4F,cocos2d::ccColor4F> color4fVarFromValue(const Json::Value& value);
    static cocos2d::ccColor4F color4fFromValue(const Json::Value& value);
    static unsigned int hashFromString(const char* string);
    static bool getParentPath(const std::string& fullname,std::string& parent);
};

#endif
