//
//  CCBJsonUtils.cpp
//  CCBJsonExample
//
//  Created by 靖宇 张 on 12-4-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCBJsonUtils.h"

using namespace cocos2d;
using namespace Json;

bool CCBJsonUtils::valueFromFile(const char *file, Value &value) {
    bool result = false;
    unsigned char* data = NULL;
    
    do {
        std::string fullPath = CCFileUtils::fullPathFromRelativePath(file);
        unsigned long length;
        data = CCFileUtils::getFileData(fullPath.c_str(), "r", &length);
        CC_BREAK_IF(!data);
        
        Reader reader;
        CC_BREAK_IF(!reader.parse(reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data + length),value,false));
        
        result = true;
    } while (0);
    
    delete[] data;
    return result;
}

// BKDHash
unsigned int CCBJsonUtils::hashFromString(const char *string) {
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
    
	while (*string)
		hash = hash * seed + (*string++);
    
	return (hash & 0x7FFFFFFF);
}


CCPoint CCBJsonUtils::pointFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // Point is an array
    return CCPointMake(value[0].asFloat(),value[1].asFloat());   
}

CCSize CCBJsonUtils::sizeFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // Size is an array
    return CCSizeMake(value[0].asFloat(), value[1].asFloat());
}

CCBJsonUtils::ScaleLockType CCBJsonUtils::scaleFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 3);   // Size is an array
    ScaleLockType scaling = { value[0].asFloat(), value[1].asFloat(), value[2].asBool() };
    return scaling;
}

std::pair<std::string, std::string> CCBJsonUtils::displayFrameFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // displayFrame is an array
    return std::make_pair(value[0].asString(), value[1].asString());
}

ccColor3B CCBJsonUtils::color3bFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 3);   // color is an array
    return ccc3(value[0].asUInt(), value[1].asUInt(), value[2].asUInt());
}

std::pair<bool, bool> CCBJsonUtils::flipFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // flip is an array
    return std::make_pair(value[0].asBool(), value[1].asBool());
}

ccBlendFunc CCBJsonUtils::blendFuncFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // blendFunc is an array
    ccBlendFunc bf = { value[0].asInt(),value[1].asInt() };
    return bf;
}

std::pair<float,float> CCBJsonUtils::floatVarFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // floatVar is an array
    return std::make_pair(value[0].asFloat(), value[1].asFloat());
}

std::pair<ccColor4F, ccColor4F> CCBJsonUtils::color4fVarFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 2);   // color4fVar is an array
    return std::make_pair(color4fFromValue(value[0]), color4fFromValue(value[1]));
}

ccColor4F CCBJsonUtils::color4fFromValue(const Json::Value &value) {
    CC_ASSERT(value.size() == 4); 
    ccColor4F color = { value[0].asFloat(),value[1].asFloat(),value[2].asFloat(),value[3].asFloat() };
    return color;
}

bool CCBJsonUtils::getParentPath(const std::string &fullname,std::string& parent) {    
    bool found = false;
    std::string::size_type pos = fullname.rfind('/');
    if(pos != std::string::npos) {
        parent = fullname.substr(0,pos);
        found = true;
    }
    return found;
}


