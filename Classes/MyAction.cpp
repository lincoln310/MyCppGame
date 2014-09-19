//
//  MyAction.cpp
//  Tutorial1
//
//  Created by Apple on 14-8-15.
//
//

#include "MyAction.h"

USING_NS_CC;

CircleBy *CircleBy::create(float dt, cocos2d::Vec2 center)
{
    CircleBy *pCircleBy = new CircleBy();
    if(pCircleBy)
    {
        pCircleBy->init(dt, center);
        pCircleBy->autorelease();
    }
    
    return pCircleBy;
}

bool CircleBy::init(float dt, Vec2 center)
{
    if(ActionInterval::initWithDuration(dt))
    {
        m_circleCenter = center;
        m_radian = 2.0 * M_PI/(dt/(Director::sharedDirector()->getAnimationInterval()));

        return true;
    }
    return false;
}

void CircleBy::update(float dt)
{
    auto newPos = _target->getPosition();
    auto oldPos = newPos;
    newPos.rotate(m_circleCenter, m_radian);
    
    auto rot = (newPos - m_circleCenter).getAngle();
    rot *= (180 / M_PI);
    
    _target->setRotation(-rot + 90);
    _target->setPosition(newPos);

}