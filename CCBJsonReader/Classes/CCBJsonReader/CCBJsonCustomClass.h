//
//  CCBJsonCustomClass.h
//  CCBJsonReader
//
//  Created by 靖宇 张 on 12-4-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CCBJsonReader_CCBJsonCustomClass_h
#define CCBJsonReader_CCBJsonCustomClass_h


#include "cocos2d.h"
#include "Json/json.h"
#include "string"
#include "map"


typedef cocos2d::CCNode* (* NodeFunc)(const Json::Value& value);


#define CCBJsonRegisterCustomClass(name,class)  \
    CCBJsonCustomClass::sharedCustomClass()->addCustomClassForName((name),(class))

class CCBJsonCustomClass {
public:
    static CCBJsonCustomClass* sharedCustomClass();
    static void purgeCustomClass();
    
    void addCustomClassForName(const std::string& name,NodeFunc func) {
        customNodes_[name] = func;
    }
    
    void removeCustomClassForName(const std::string& name);
    
    NodeFunc customClassForName(const std::string& name) const {
        std::map< std::string, NodeFunc >::const_iterator classIter = customNodes_.find(name);
        return (classIter == customNodes_.end()) ? NULL : classIter->second;
    }
protected:
    std::map< std::string, NodeFunc > customNodes_;
    
private:
    CCBJsonCustomClass() { }
};



#endif
