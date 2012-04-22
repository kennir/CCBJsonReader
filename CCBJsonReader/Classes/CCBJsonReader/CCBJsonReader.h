//
//  CCBJsonReader.h
//  CCBJsonExample
//
//  Created by 靖宇 张 on 12-4-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CCBJsonExample_CCBJsonReader_h
#define CCBJsonExample_CCBJsonReader_h

#include "cocos2d.h"
#include "json/json.h"
#include "CCBJsonCustomClass.h"
#include "CCBJsonSelectorManager.h"



class CCBJsonReader 
{
public:
    typedef enum 
    {
        kNodeTypeUnknown,
        kNodeTypeCCLayer,
        kNodeTypeCCLayerColor,
        kNodeTypeCCLayerGradient,
        kNodeTypeCCSprite,
        kNodeTypeCCParticleSystemQuad,
        kNodeTypeCCLabelBMFont,
        kNodeTypeCCLabelTTF,
        kNodeTypeCCMenu,
        kNodeTypeCCMenuItemImage,
        kNodeTypeCCBFile,
    } NodeType;
    
public:
    static cocos2d::CCNode* nodeGraphFromFile(const char* file);
    static cocos2d::CCScene* sceneWithNodeGraphFromFile(const char* file);
public:
    CCBJsonReader();
    virtual ~CCBJsonReader();
    
    bool initWithFile(const char* file);
    cocos2d::CCNode* getNodeGraph() const { return nodeGraph_; }
    

protected:
    cocos2d::CCNode* nodeFromValue(const Json::Value& value); 
    // create node from base class
    cocos2d::CCNode* nodeFromBaseClass(const std::string& baseClass,
                                       const std::string& customClass,
                                       const Json::Value& value);
    
    void readPropertyForNode(cocos2d::CCNode* node,const Json::Value& property);
    
    cocos2d::CCSprite* spriteFromValue(const Json::Value& value);
    cocos2d::CCSpriteFrame* spriteFrameFromValue(const Json::Value& value);
    cocos2d::CCParticleSystemQuad* particleSystemFromValue(const Json::Value& value);
    cocos2d::CCLabelBMFont* labelBMFontFromValue(const Json::Value& value);
    cocos2d::CCLabelTTF* labelTTFFromValue(const Json::Value& value);
    cocos2d::CCNode* ccbFileNodeFromValue(const Json::Value& value);
    cocos2d::CCMenuItemImage* menuItemImageFromValue(const Json::Value& value);
    
    Json::Value findPropertyByName(const Json::Value& properties,const char* name) const;
    
    
protected:
    cocos2d::CCNode* nodeGraph_;
    
private:
    NodeType typeReading_;

};



#endif
