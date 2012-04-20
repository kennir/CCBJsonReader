//
//  TestHeader.h
//  CCBJsonReader
//
//  Created by 靖宇 张 on 12-4-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CCBJsonReader_TestHeader_h
#define CCBJsonReader_TestHeader_h

#include "cocos2d.h"
#include "json/json.h"

class TestHeader : public cocos2d::CCLayer{
public:
    LAYER_NODE_FUNC(TestHeader)
    
    static cocos2d::CCNode* nodeFunc(const Json::Value& value) { 
        return TestHeader::node();
    }
    
public:
    virtual bool init() {
        CCLOG("TestHeader init");
        
        bool result = false;
        do {
            // Register menu handler
            CCBJsonRegisterMenuHandler("pressedBack:",this,(cocos2d::SEL_MenuHandler)(&TestHeader::pressBack));
            
            result = true;
        } while (0);
        return result;
    }
    
    virtual ~TestHeader() {
        CCBJsonClearSelectorAndHandlers();
    }
    
    void pressBack(cocos2d::CCObject* sender) {
        CCLOG("Menu pressed");
    }
};


#endif
