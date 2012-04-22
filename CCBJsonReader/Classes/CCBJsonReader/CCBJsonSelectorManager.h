//
//  CCBJsonSelectorManager.h
//  CCBJsonReader
//
//  Created by 靖宇 张 on 12-4-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CCBJsonReader_CCBJsonSelectorManager_h
#define CCBJsonReader_CCBJsonSelectorManager_h

#include "cocos2d.h"


#define CCBJsonRegisterMenuItem(item,sel)    \
    CCBJsonSelectorManager::sharedSelectorManager()->registerSelectorForMenuItem((item),(sel))
#define CCBJsonRegisterMenuHandler(sel,target,handler)    \
    CCBJsonSelectorManager::sharedSelectorManager()->registerHandlerForSelector((sel),(target),(handler))
#define CCBJsonClearSelectorAndHandlers() \
    CCBJsonSelectorManager::sharedSelectorManager()->clearMenuSelectorAndHandlers()

class CCBJsonSelectorManager : public cocos2d::CCObject 
{
public:
    static CCBJsonSelectorManager* sharedSelectorManager();
    static void purgeSelectorManager();
public:
    void registerSelectorForMenuItem(cocos2d::CCObject* item,const std::string& selector);
    void registerHandlerForSelector(const std::string& selector,cocos2d::CCObject* target,cocos2d::SEL_MenuHandler handler);
    
    void clearMenuSelectorAndHandlers();

    void defaultMenuHandler(cocos2d::CCObject* sender);
protected:
    std::map<cocos2d::CCObject*,std::string> menuSelector_;
    std::map<std::string, std::pair<cocos2d::CCObject*, cocos2d::SEL_MenuHandler> > menuHandlers_;
};


#endif
