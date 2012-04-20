//
//  CCBJsonReader.cpp
//  CCBJsonExample
//
//  Created by 靖宇 张 on 12-4-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCBJsonReader.h"
#include "CCBJsonUtils.h"


using namespace cocos2d;
using namespace Json;

#pragma mark Hash table

// CCNode
static const unsigned int _hashPosition = CCBJsonUtils::hashFromString("position");
static const unsigned int _hashContentSize = CCBJsonUtils::hashFromString("contentSize");
static const unsigned int _hashAnchorPoint = CCBJsonUtils::hashFromString("anchorPoint");
static const unsigned int _hashScale = CCBJsonUtils::hashFromString("scale");
static const unsigned int _hashRotation = CCBJsonUtils::hashFromString("rotation");
static const unsigned int _hashTag = CCBJsonUtils::hashFromString("tag");
static const unsigned int _hashIsRelativeAnchorPoint = CCBJsonUtils::hashFromString("isRelativeAnchorPoint");
static const unsigned int _hashVisible = CCBJsonUtils::hashFromString("visible");

// CCLayer
static const unsigned int _hashIsTouchEnabled = CCBJsonUtils::hashFromString("isTouchEnabled");
static const unsigned int _hashIsAccelerometerEnabled = CCBJsonUtils::hashFromString("isAccelerometerEnabled");
static const unsigned int _hashIsMouseEnabled = CCBJsonUtils::hashFromString("isMouseEnabled");
static const unsigned int _hashIsKeyboardEnabled = CCBJsonUtils::hashFromString("isKeyboardEnabled");

// CCSprite
static const unsigned int _hashDisplayFrame = CCBJsonUtils::hashFromString("displayFrame");
static const unsigned int _hashFlip = CCBJsonUtils::hashFromString("flip");

// CCRGBAProcotol
static const unsigned int _hashOpacity = CCBJsonUtils::hashFromString("opacity");
static const unsigned int _hashColor = CCBJsonUtils::hashFromString("color");

// CCBlendProcotol
static const unsigned int _hashBlendFunc = CCBJsonUtils::hashFromString("blendFunc");

// CCLayerGradient
static const unsigned int _hashStartColor = CCBJsonUtils::hashFromString("startColor");
static const unsigned int _hashStartOpacity = CCBJsonUtils::hashFromString("startOpacity");
static const unsigned int _hashEndColor = CCBJsonUtils::hashFromString("endColor");
static const unsigned int _hashEndOpacity = CCBJsonUtils::hashFromString("endOpacity");
static const unsigned int _hashVector = CCBJsonUtils::hashFromString("vector");

// CCParticleSystemQuad
static const unsigned int _hashEmitterMode = CCBJsonUtils::hashFromString("emitterMode");
static const unsigned int _hashPosVar = CCBJsonUtils::hashFromString("posVar");
static const unsigned int _hashEmissionRate = CCBJsonUtils::hashFromString("emissionRate");
static const unsigned int _hashDuration = CCBJsonUtils::hashFromString("duration");
static const unsigned int _hashTotalParticles = CCBJsonUtils::hashFromString("totalParticles");
static const unsigned int _hashLife = CCBJsonUtils::hashFromString("life");
static const unsigned int _hashStartSize = CCBJsonUtils::hashFromString("startSize");
static const unsigned int _hashEndSize = CCBJsonUtils::hashFromString("endSize");
static const unsigned int _hashStartSpin = CCBJsonUtils::hashFromString("startSpin");
static const unsigned int _hashEndSpin = CCBJsonUtils::hashFromString("endSpin");
static const unsigned int _hashAngle = CCBJsonUtils::hashFromString("angle");
static const unsigned int _hashGravity = CCBJsonUtils::hashFromString("gravity");
static const unsigned int _hashSpeed = CCBJsonUtils::hashFromString("speed");
static const unsigned int _hashTangentialAccel = CCBJsonUtils::hashFromString("tangentialAccel");
static const unsigned int _hashRadialAccel = CCBJsonUtils::hashFromString("radialAccel");
static const unsigned int _hashTexture = CCBJsonUtils::hashFromString("texture");

// CCLabelBMFont
static const unsigned int _hashFntFile = CCBJsonUtils::hashFromString("fntFile");
static const unsigned int _hashString = CCBJsonUtils::hashFromString("string");

// CCLabelTTF
static const unsigned int _hashFontName = CCBJsonUtils::hashFromString("fontName");
static const unsigned int _hashFontSize = CCBJsonUtils::hashFromString("fontSize");
static const unsigned int _hashDimensions = CCBJsonUtils::hashFromString("dimensions");
static const unsigned int _HashHorizontalAlignment = CCBJsonUtils::hashFromString("horizontalAlignment");
static const unsigned int _hashVerticalAlignment = CCBJsonUtils::hashFromString("verticalAlignment");


// CCBFile
static const unsigned int _hashCCBFile = CCBJsonUtils::hashFromString("ccbFile");


// CCMenu

// CCMenuItem
static const unsigned int _hashIsEnabled = CCBJsonUtils::hashFromString("isEnabled");
// CCMenuItemImage
static const unsigned int _hashNormalSpriteFrame = CCBJsonUtils::hashFromString("normalSpriteFrame");
static const unsigned int _hashSelectedSpriteFrame = CCBJsonUtils::hashFromString("selectedSpriteFrame");
static const unsigned int _hashDisabledSpriteFrame = CCBJsonUtils::hashFromString("disabledSpriteFrame");



#pragma mark class functions

CCNode* CCBJsonReader::nodeGraphFromFile(const char *file) {
    CCNode* node = NULL;
    CCBJsonReader reader;
    if(reader.initWithFile(file)) {
        node = reader.getNodeGraph();
    }

    return node;
}

CCScene* CCBJsonReader::sceneWithNodeGraphFromFile(const char *file) {
    CCScene* scene = CCScene::node();
    CCBJsonReader reader;
    if(reader.initWithFile(file)) {
        scene->addChild(reader.getNodeGraph());
    }
    return scene;
}


CCBJsonReader::CCBJsonReader() 
: nodeGraph_(NULL)
, typeReading_(kNodeTypeUnknown) {
    
}

CCBJsonReader::~CCBJsonReader() {
    CC_SAFE_RELEASE(nodeGraph_);
}




bool CCBJsonReader::initWithFile(const char *file) {
    bool result = false;
    do {
        std::string fullPath(file);
        std::string ccbExt("ccb");
        if(fullPath.compare(fullPath.size() - ccbExt.size(), ccbExt.size(), ccbExt) == 0) {
            fullPath += "json";
        }
        
        
        
        Value value;
        CC_BREAK_IF(!CCBJsonUtils::valueFromFile(fullPath.c_str(), value));
        
        // Check file version
        Value& version = value["fileVersion"];
        CC_BREAK_IF(version.asInt() != 3);  // Current version is 3
        
        Value& ng = value["nodeGraph"];
        CC_BREAK_IF(ng.isNull());
        
        nodeGraph_ = nodeFromValue(ng);
        CC_BREAK_IF(!nodeGraph_);
        nodeGraph_->retain();
        
        result = true;
    } while (0);
    return result;
}

CCNode* CCBJsonReader::nodeFromValue(const Json::Value &value) {
    CCNode* node = NULL;
    
    std::string baseClass = value["baseClass"].asString();
    std::string customClass = value["customClass"].asString();
    node = nodeFromBaseClass(baseClass, customClass, value);
    if(node) {
        const Value& properties = value["properties"];
        for(ArrayIndex index = 0; index < properties.size(); ++index) 
            readPropertyForNode(node, properties[index]);
        
        // read children
        const Value& children = value["children"];
        for(ArrayIndex index = 0; index < children.size(); ++index) {
            CCNode* child = nodeFromValue(children[index]);
            if(child)
                node->addChild(child);
        }
    }

    return node;
}


CCNode* CCBJsonReader::nodeFromBaseClass(const std::string &baseClass, const std::string &customClass, const Json::Value &value) {
    
    if(!customClass.empty()) {
        NodeFunc func = CCBJsonCustomClass::sharedCustomClass()->customClassForName(customClass);
        if(func)
            return (*func)(value);
    }
    
    CCNode* node = NULL;
    
    if(!baseClass.compare("CCLayer")) {
        node = CCLayer::node();
        typeReading_ = kNodeTypeCCLayer;
        CCLOG("add CCLayer");
    } else if(!baseClass.compare("CCSprite")) {
        node = spriteFromValue(value);
        typeReading_ = kNodeTypeCCSprite;
        CCLOG("add CCSprite");
    } else if(!baseClass.compare("CCLayerColor")) {
        node = CCLayerColor::layerWithColor(ccc4f(255, 255, 255, 255));
        typeReading_ = kNodeTypeCCLayerColor;
        CCLOG("add CCLayerColor");
    } else if(!baseClass.compare("CCLayerGradient")) {
        node = CCLayerGradient::layerWithColor(ccc4f(255, 255, 255, 255), ccc4f(255, 255, 255, 255));
        typeReading_ = kNodeTypeCCLayerGradient;
        CCLOG("add CCLayerGradient");
    } else if(!baseClass.compare("CCMenu")) {
        node = CCMenu::node();
        typeReading_ = kNodeTypeCCMenu;
        CCLOG("add CCMenu"); 
    } else if(!baseClass.compare("CCMenuItemImage")) {
        node = menuItemImageFromValue(value);
        typeReading_ = kNodeTypeCCMenuItemImage;
        CCLOG("add CCMenuItemImage"); 
    } else if(!baseClass.compare("CCParticleSystemQuad")) {
        node = particleSystemFromValue(value);
        typeReading_ = kNodeTypeCCParticleSystemQuad;
        CCLOG("add CCParticleSystemQuad");
    } else if(!baseClass.compare("CCLabelBMFont")) {
        node = labelBMFontFromValue(value);
        typeReading_ = kNodeTypeCCLabelBMFont;
        CCLOG("add CCLabelBMFont");
    } else if(!baseClass.compare("CCLabelTTF")) {
        node = labelTTFFromValue(value);
        typeReading_ = kNodeTypeCCLabelTTF;
        CCLOG("add CCLabelBMFont");
    } else if(!baseClass.compare("CCBFile")) {
        node = ccbFileNodeFromValue(value);
        typeReading_ = kNodeTypeCCBFile;
        CCLOG("add CCBFile");
    } else {
        CCLOG("Unknown base class: %s",baseClass.c_str());
        typeReading_ = kNodeTypeUnknown;
    }
    return node;
}


void CCBJsonReader::readPropertyForNode(cocos2d::CCNode *node, const Json::Value &property) {
    std::string name = property["name"].asString();
    unsigned int nameHash = CCBJsonUtils::hashFromString(name.c_str());
    
    const Value& value = property["value"];
    
    if(nameHash == _hashPosition) {
        node->setPosition(CCBJsonUtils::pointFromValue(value));
    } else if(nameHash == _hashContentSize) {
        node->setContentSize(CCBJsonUtils::sizeFromValue(value));
    } else if(nameHash == _hashAnchorPoint) {
        node->setAnchorPoint(CCBJsonUtils::pointFromValue(value));
    } else if(nameHash == _hashScale) {
        CCBJsonUtils::ScaleLockType scaling = CCBJsonUtils::scaleFromValue(value);
        node->setScaleX(scaling.scaleX);
        node->setScaleY(scaling.scaleY);
    } else if(nameHash == _hashRotation) {
        node->setRotation(value.asFloat());
    } else if(nameHash == _hashTag) {
        node->setTag(value.asInt());
    } else if(nameHash == _hashIsRelativeAnchorPoint) {
        node->setIsRelativeAnchorPoint(value.asBool());
    } else if(nameHash == _hashVisible) {
        node->setIsVisible(value.asBool());
    } else if(nameHash == _hashIsTouchEnabled) {
        CCLayer* layer = dynamic_cast<CCLayer*>(node);      
        CC_ASSERT(layer);
        layer->setIsTouchEnabled(value.asBool());
    } else if(nameHash == _hashIsAccelerometerEnabled) {
        CCLayer* layer = dynamic_cast<CCLayer*>(node);      
        CC_ASSERT(layer);
        layer->setIsAccelerometerEnabled(value.asBool());
    } else if(nameHash == _hashIsMouseEnabled) {
    } else if(nameHash == _hashIsKeyboardEnabled) {
    } else if(nameHash == _hashDisplayFrame) {
        CCSprite* sprite = dynamic_cast<CCSprite*>(node);
        CC_ASSERT(sprite);
        sprite->setDisplayFrame(spriteFrameFromValue(value));
    } else if(nameHash == _hashFlip) {
        CCSprite* sprite = dynamic_cast<CCSprite*>(node);
        CC_ASSERT(sprite);
        std::pair<bool,bool> flip = CCBJsonUtils::flipFromValue(value);
        sprite->setFlipX(flip.first);
        sprite->setFlipY(flip.second);
    } else if(nameHash == _hashOpacity) {
        CCRGBAProtocol* protocol = dynamic_cast<CCRGBAProtocol*>(node);
        CC_ASSERT(protocol);
        protocol->setOpacity(static_cast<GLbyte>(value.asUInt()));
    } else if(nameHash == _hashColor) {
        CCRGBAProtocol* protocol = dynamic_cast<CCRGBAProtocol*>(node);
        CC_ASSERT(protocol);
        protocol->setColor(CCBJsonUtils::color3bFromValue(value));
    } else if(nameHash == _hashBlendFunc) {
        CCBlendProtocol* protocol = dynamic_cast<CCBlendProtocol*>(node);
        CC_ASSERT(protocol);
        protocol->setBlendFunc(CCBJsonUtils::blendFuncFromValue(value));
    } else if(nameHash == _hashStartColor) {
        if(typeReading_ == kNodeTypeCCParticleSystemQuad) {
            CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
            CC_ASSERT(quad);
            std::pair<ccColor4F,ccColor4F> colorVar = CCBJsonUtils::color4fVarFromValue(value);
            quad->setStartColor(colorVar.first);
            quad->setStartColorVar(colorVar.second);
        } else {
            CCLayerGradient* layer = dynamic_cast<CCLayerGradient*>(node);
            CC_ASSERT(layer);
            layer->setStartColor(CCBJsonUtils::color3bFromValue(value));
        }
    } else if(nameHash == _hashStartOpacity) {
        CCLayerGradient* layer = dynamic_cast<CCLayerGradient*>(node);
        CC_ASSERT(layer);
        layer->setStartOpacity(static_cast<GLbyte>(value.asUInt()));
    } else if(nameHash == _hashEndColor) {
        if(typeReading_ == kNodeTypeCCParticleSystemQuad) {
            CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
            CC_ASSERT(quad);
            std::pair<ccColor4F,ccColor4F> colorVar = CCBJsonUtils::color4fVarFromValue(value);
            quad->setEndColor(colorVar.first);
            quad->setEndColorVar(colorVar.second);
        } else {
            CCLayerGradient* layer = dynamic_cast<CCLayerGradient*>(node);
            CC_ASSERT(layer);
            layer->setEndColor(CCBJsonUtils::color3bFromValue(value));
        }
    } else if(nameHash == _hashEndOpacity) {
        CCLayerGradient* layer = dynamic_cast<CCLayerGradient*>(node);
        CC_ASSERT(layer);
        layer->setEndOpacity(static_cast<GLbyte>(value.asUInt()));
    } else if(nameHash == _hashVector) {
        CCLayerGradient* layer = dynamic_cast<CCLayerGradient*>(node);
        CC_ASSERT(layer);
        layer->setVector(CCBJsonUtils::pointFromValue(value));
    } else if(nameHash == _hashEmitterMode) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        quad->setEmitterMode(value.asInt());
    } else if(nameHash == _hashPosVar) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        quad->setPosVar(CCBJsonUtils::pointFromValue(value)); 
    } else if(nameHash == _hashEmissionRate) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        quad->setEmissionRate(value.asFloat());
    } else if(nameHash == _hashDuration) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        quad->setDuration(value.asFloat());
    } else if(nameHash == _hashTotalParticles) {
        // do nothing
    } else if(nameHash == _hashLife) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setLife(floatVar.first);
        quad->setLifeVar(floatVar.second);
    } else if(nameHash == _hashStartSize) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setStartSize(floatVar.first);
        quad->setStartSizeVar(floatVar.second);
    } else if(nameHash == _hashEndSize) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setEndSize(floatVar.first);
        quad->setEndSizeVar(floatVar.second);
    } else if(nameHash == _hashStartSpin) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setStartSpin(floatVar.first);
        quad->setStartSpinVar(floatVar.second);
    } else if(nameHash == _hashEndSpin) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setEndSpin(floatVar.first);
        quad->setEndSpinVar(floatVar.second);
    } else if(nameHash == _hashAngle) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setAngle(floatVar.first);
        quad->setAngleVar(floatVar.second);
    } else if(nameHash == _hashGravity) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        quad->setGravity(CCBJsonUtils::pointFromValue(value));
    } else if(nameHash == _hashSpeed) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setSpeed(floatVar.first);
        quad->setSpeedVar(floatVar.second);
    } else if(nameHash == _hashTangentialAccel) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setTangentialAccel(floatVar.first);
        quad->setTangentialAccelVar(floatVar.second);
    } else if(nameHash == _hashRadialAccel) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        std::pair<float, float> floatVar = CCBJsonUtils::floatVarFromValue(value);
        quad->setRadialAccel(floatVar.first);
        quad->setRadialAccelVar(floatVar.second);
    } else if(nameHash == _hashTexture) {
        CCParticleSystemQuad* quad = dynamic_cast<CCParticleSystemQuad*>(node);
        CC_ASSERT(quad);
        quad->setTexture(cocos2d::CCTextureCache::sharedTextureCache()->addImage(value.asString().c_str()));
    } else if(nameHash == _hashString) {
    } else if(nameHash == _hashFntFile) {
    } else if(nameHash == _hashFontName) {
    } else if(nameHash == _hashFontSize) {
    } else if(nameHash == _hashDimensions) {
    } else if(nameHash == _HashHorizontalAlignment) {
    } else if(nameHash == _hashVerticalAlignment) {
    } else if(nameHash == _hashCCBFile) {
    } else if(nameHash == _hashIsEnabled) {
        CCMenuItem* item = dynamic_cast<CCMenuItem*>(node);
        CC_ASSERT(item);
        item->setIsEnabled(value.asBool());
    } else if(nameHash == _hashNormalSpriteFrame) {
    } else if(nameHash == _hashSelectedSpriteFrame) {
    } else if(nameHash == _hashDisabledSpriteFrame) {
    } else {
        CCLOG("Unknown property named: %s",name.c_str());
    }
}


CCSpriteFrame* CCBJsonReader::spriteFrameFromValue(const Json::Value &value) {
    std::pair<std::string, std::string> displayFrame = CCBJsonUtils::displayFrameFromValue(value);
    
    CCSpriteFrame* frame = NULL;
    if(displayFrame.first.empty()) {
        CCTexture2D* t = cocos2d::CCTextureCache::sharedTextureCache()->addImage(displayFrame.second.c_str());
        CCSize textContentSize = t->getContentSize();
        CCRect textRect = cocos2d::CCRectMake(0, 0, textContentSize.width, textContentSize.height);
        frame = CCSpriteFrame::frameWithTexture(t, textRect);
    } else {
        CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile(displayFrame.first.c_str());
        frame = cache->spriteFrameByName(displayFrame.second.c_str());
    }
    CC_ASSERT(frame);
    
    return frame;
}

CCSprite* CCBJsonReader::spriteFromValue(const Json::Value &value) {
    CCSprite* sprite = new CCSprite;
    if(sprite && sprite->init()){
        sprite->autorelease();
    } else {
        delete sprite;
        sprite = NULL;
    }
    return sprite;
}


CCParticleSystemQuad* CCBJsonReader::particleSystemFromValue(const Json::Value &value) {
    
    Value totalParticles = findPropertyByName(value["properties"], "totalParticles");
    CC_ASSERT(!totalParticles.isNull());
    
    CCParticleSystemQuad* particles = new CCParticleSystemQuad;
    if(particles && particles->initWithTotalParticles(totalParticles["value"].asUInt()))
        particles->autorelease();
    else {
        delete particles;
        particles = NULL;
    }

    return particles;
}


CCLabelBMFont* CCBJsonReader::labelBMFontFromValue(const Json::Value &value) {
    CCLabelBMFont* font = NULL;
    
    const Value& properties = value["properties"];
    Value fntFile = findPropertyByName(properties, "fntFile");
    CC_ASSERT(!fntFile.isNull());
    
    Value string = findPropertyByName(properties, "string");
    if(!fntFile.isNull()) {
        font = CCLabelBMFont::labelWithString((!string.isNull()) ? string["value"].asString().c_str() : "", 
                                              fntFile["value"].asString().c_str());
    }
    return font;
}


CCLabelTTF* CCBJsonReader::labelTTFFromValue(const Json::Value &value) {
    CCLabelTTF* label = NULL;

    const Value& properties = value["properties"];
    Value string = findPropertyByName(properties, "string");
    Value fontName = findPropertyByName(properties, "fontName");
    Value fontSize = findPropertyByName(properties, "fontSize");
    Value dimensions = findPropertyByName(properties, "dimensions");
    Value alignment = findPropertyByName(properties, "horizontalAlignment");
    
    if(!string.isNull() && !fontName.isNull() && !fontSize.isNull() && !dimensions.isNull()) {
        label = CCLabelTTF::labelWithString(string["value"].asString().c_str(), 
                                            CCBJsonUtils::sizeFromValue(dimensions["value"]), 
                                            static_cast<CCTextAlignment>(alignment["value"].asInt()), 
                                            fontName["value"].asString().c_str(), 
                                            fontSize["value"].asFloat());
    }

    return label;
}

CCNode* CCBJsonReader::ccbFileNodeFromValue(const Json::Value &value) {
    CCNode* node = NULL;
    
    Value ccbFile = findPropertyByName(value["properties"], "ccbFile");
    if(!ccbFile.isNull()) {
        node = CCBJsonReader::nodeGraphFromFile(ccbFile["value"].asString().c_str());
    }
    
    return node;
}

CCMenuItemImage* CCBJsonReader::menuItemImageFromValue(const Json::Value &value) {
    CCMenuItemImage* item = NULL;
    
    const Value& properties = value["properties"];
    Value normal = findPropertyByName(properties, "normalSpriteFrame");
    Value selected = findPropertyByName(properties, "selectedSpriteFrame");
    Value disabled = findPropertyByName(properties, "disabledSpriteFrame");
    
    CCSpriteFrame* normalFrame = spriteFrameFromValue(normal["value"]);
    CCSpriteFrame* selectedFrame = spriteFrameFromValue(selected["value"]);
    CCSpriteFrame* disabledFrame = spriteFrameFromValue(disabled["value"]);
    
    CCSprite* normalSprite = (normalFrame) ? CCSprite::spriteWithSpriteFrame(normalFrame) : NULL;
    CCSprite* selectedSprite = (selectedFrame) ? CCSprite::spriteWithSpriteFrame(selectedFrame) : NULL;
    CCSprite* disabledSprite = (disabledFrame) ? CCSprite::spriteWithSpriteFrame(disabledFrame) : NULL;
    
    item = new CCMenuItemImage();
    if(item && item->initFromNormalSprite(normalSprite, selectedSprite, disabledSprite, NULL, NULL))
        item->autorelease();
    else {
        delete item;
        item = NULL;
    }

    return item;
}


//CCControlButton* 
//CCBJsonReader::controlButtonFromBaseClass(const std::string &baseClass, const std::string &customClass, const Json::Value &value){
//    CCControlButton* button = NULL;
//
//    const Value& properties = value["properties"];
//    Value title = findPropertyByName(properties, "title|1");
//    Value font = findPropertyByName(properties, "titleTTF|1");
//    Value fontSize = findPropertyByName(properties, "titleTTFSize|1");
//    
//    if(!title.isNull() && !font.isNull() && !fontSize.isNull()) {
//        button = CCControlButton::buttonWithTitleAndFontNameAndFontSize(title["value"].asString(), 
//                                                                        font["value"].asString().c_str(), 
//                                                                        fontSize["value"].asFloat());
//        CC_ASSERT(button);
//    }
//    
//    
//    return button;
//}

Value CCBJsonReader::findPropertyByName(const Value &properties, const char* name) const {
    Value value;
    for(ArrayIndex index = 0; index < properties.size(); ++index) {
        const Value& property = properties[index];
        if(!property["name"].asString().compare(name)) {
            value = property;
            break;
        }
    }
    return value;
}






