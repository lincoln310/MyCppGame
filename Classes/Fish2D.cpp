//
//  Fish2D.cpp
//  Tutorial1
//
//  Created by Apple on 14-8-15.
//
//

#include <stdio.h>
#include "Fish2D.h"


USING_NS_CC;

Fish::Fish()
: m_Color(cocos2d::Color3B(255,255,255)), m_EatCount(1), m_Count(0)
{
    
}

Fish *Fish::create(Fish *fish)
{
    if(fish && fish->initWithFile(fish->m_ModelFile))
    {
        auto body = PhysicsBody::createCircle(fish->getContentSize().width/4);
        
        fish->setPhysicsBody(body);
        fish->setBodyContactMask();
        fish->autorelease();
        return fish;
    }
    CC_SAFE_DELETE(fish);
    return nullptr;
}

bool Fish::initWithFile(std::string file)
{
    Sprite::initWithFile(file);
    setColor(m_Color);
    
    if(m_SpriteType <= _ST_Burden)
    {
        m_Count = LabelAtlas::create("0:", "numbersPic.png", 14, 21, '0');
        
        m_Count->setPosition(Vec2::ZERO);
        addChild(m_Count);
    }
    
    scheduleUpdate();
    return true;
}

void Fish::update(float dt)
{
    if(m_Count)
    {
        char str[3] = {0};
        sprintf(str, "%d", m_EatCount);

        m_Count->setString(str);
    }
}

void Fish::setBodyContactMask()
{
    auto body = getPhysicsBody();
    body->setCategoryBitmask(m_SpriteType);
    body->setContactTestBitmask(_ST_End - m_SpriteType - _ST_Wall);
    body->setCollisionBitmask(_ST_End - m_SpriteType);
    
}

Scissor::Scissor()
{
    m_ModelFile = "scissor.png";
    m_SpriteType = _ST_Scissor;
    m_Color = Color3B(242,88,77);
}

Hummer::Hummer()
{
    m_ModelFile = "hammer.png";
    m_SpriteType = _ST_Hummer;
    m_Color = Color3B(250, 203, 118);
}

Burden::Burden()
{
    m_ModelFile = "burden.png";
    m_SpriteType = _ST_Burden;
    m_Color = Color3B(73,145,1);
}

Wall::Wall()
{
    m_ModelFile = "wall.png";
    m_SpriteType = _ST_Wall;
}

Dest::Dest()
{
    m_ModelFile = "dest.png";
    m_SpriteType = _ST_Dest;
}

void Wall::setBodyContactMask()
{
    auto body = getPhysicsBody();
    body->setCategoryBitmask(_ST_Wall);
    body->setCollisionBitmask(_ST_End);
}

MySprite *MySprite2DFactory::Create(int type)
{
    MySprite *ret = nullptr;
    switch (type) {
        case _ST_Wall:
            ret = new Wall();
            break;
        case _ST_Scissor:
            ret = new Scissor();
            break;
        case _ST_Hummer:
            ret = new Hummer();
            break;
        case _ST_Burden:
            ret = new Burden();
            break;
        case _ST_Dest:
            ret = new Dest();
            break;
        default:
            break;
    }
    if(ret)
        Fish::create(ret);
    return ret;
}