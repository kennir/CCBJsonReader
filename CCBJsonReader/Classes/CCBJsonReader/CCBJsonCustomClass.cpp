//
//  CCBJsonCustomClass.cpp
//  CCBJsonReader
//
//  Created by 靖宇 张 on 12-4-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCBJsonCustomClass.h"

static CCBJsonCustomClass* _customClass = NULL;

CCBJsonCustomClass* CCBJsonCustomClass::sharedCustomClass() {
    if(!_customClass) {
        _customClass = new CCBJsonCustomClass;
    }
    
    return _customClass;
}


void CCBJsonCustomClass::purgeCustomClass() {
    CC_SAFE_DELETE(_customClass);
}


void CCBJsonCustomClass::removeCustomClassForName(const std::string &name) {
    std::map<std::string, NodeFunc>::iterator classIter = customNodes_.find(name);
    if(classIter != customNodes_.end())
        customNodes_.erase(classIter);
}