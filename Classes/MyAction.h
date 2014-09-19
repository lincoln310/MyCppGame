//
//  MyAction.h
//  Tutorial1
//
//  Created by Apple on 14-8-15.
//
//

#ifndef MYACTION_H
#define MYACTION_H

#include <iostream>
#include "cocos2d.h"


class CircleBy : public cocos2d::ActionInterval
{
public:
    static CircleBy *create(float dt, cocos2d::Vec2 center);
    
    bool init(float dt, cocos2d::Vec2 center);
    
    virtual void update(float time);
    
    ActionInterval *reverse() const {return nullptr;}
    
    ActionInterval *clone() const {return nullptr;}
    
protected:
    
    cocos2d::Point m_circleCenter; //圆心坐标
    float   m_radian;       //弧度

};


#endif //MYACTION_H