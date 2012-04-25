//
//  CCBJsonSelectorManager.cpp
//  CCBJsonReader
//
//  Created by 靖宇 张 on 12-4-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCBJsonSelectorManager.h"

using namespace cocos2d;

static CCBJsonSelectorManager* _selectorManager = NULL;

CCBJsonSelectorManager* CCBJsonSelectorManager::sharedSelectorManager() {
    if(!_selectorManager)
        _selectorManager = new CCBJsonSelectorManager;
    return _selectorManager;
}


void CCBJsonSelectorManager::purgeSelectorManager() {
    CC_SAFE_DELETE(_selectorManager);
}


void CCBJsonSelectorManager::registerSelectorForMenuItem(cocos2d::CCObject *item, const std::string &selector) {
    menuSelector_[item] = selector;
}

void CCBJsonSelectorManager::registerHandlerForSelector(const std::string &selector, 
                                                        cocos2d::CCObject *target, 
                                                        cocos2d::SEL_MenuHandler handler) {
    menuHandlers_[selector] = std::make_pair(target, handler);
}


void CCBJsonSelectorManager::clearMenuSelectorAndHandlers() {
    menuSelector_.clear();
    menuHandlers_.clear();
}


void CCBJsonSelectorManager::defaultMenuHandler(cocos2d::CCObject *sender) {
    std::map<cocos2d::CCObject*,std::string>::const_iterator selectorIter = menuSelector_.find(sender);
    CC_ASSERT(selectorIter != menuSelector_.end());
    
    typedef std::map<std::string, std::pair<cocos2d::CCObject*, cocos2d::SEL_MenuHandler> > TargetMap;
    TargetMap::const_iterator handlerIter = menuHandlers_.find(selectorIter->second);
    if(handlerIter != menuHandlers_.end()) {
        const std::pair<CCObject*, SEL_MenuHandler>& targetPair = handlerIter->second;
        (targetPair.first->*targetPair.second)(sender);
    } 
    else
        CCLOG("CCBJsonSelectorManager:Can't find selector for : %s",selectorIter->second.c_str());
}

