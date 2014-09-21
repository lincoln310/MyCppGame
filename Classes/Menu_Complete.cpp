//
//  Menu_Complete.cpp
//  Tutorial1
//
//  Created by Apple on 14-9-18.
//
//

#include <stdio.h>
#include "Menu_Complete.h"
#include <sstream>
#include <time.h>

USING_NS_CC;
using namespace std;

Layer_Complete *Layer_Complete::create(FishLayer *layer)
{
    auto l = new Layer_Complete();
    
    if(l && l->init(layer))
    {
        l->autorelease();
        return l;
    }
    CC_SAFE_DELETE(l);
    return nullptr;
}

void Layer_Complete::reset()
{
    m_Layer->reset();
    removeFromParent();
}

bool Layer_Complete::init(cocos2d::FishLayer *layer)
{
    m_Layer = layer;
    setAnchorPoint(Vec2(.5,.5));
    
    auto visiableSize = Director::getInstance()->getVisibleSize();
    auto background = LayerColor::create(Color4B(128, 128, 128, 128), visiableSize.width / 2, visiableSize.height / 2);
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setPosition( -1 * visiableSize/4);
    addChild(background, -1);
    
    auto menuItem = MenuItemImage::create("reset.png", "reset.png", CC_CALLBACK_0(Layer_Complete::reset, this));
    menuItem->setAnchorPoint(Vec2(.5, .5));
    auto menu = Menu::createWithItem(menuItem);
    menu->setAnchorPoint(Vec2(.5, .5));
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey("/cc_fps_images");
    
    m_time = LabelAtlas::create();
    m_time->initWithString("0000", texture, 12, 32 , '.');
    m_time->setAnchorPoint(Vec2(0.5, 0.5));
    m_time->setPosition(menuItem->getPosition() - Vec2(0, menuItem->getContentSize().height / 2 + 17));
    addChild(m_time);
    
    m_enegy = LabelAtlas::create();
    m_enegy->initWithString("00", texture, 12, 32 , '.');
    m_enegy->setAnchorPoint(Vec2(.5, .5));
    m_enegy->setPosition(m_time->getPosition() - Vec2(0, 34));
    addChild(m_enegy);
    
    char buffer[30];

    time_t l_usedTime = m_Layer->getUsedTime();
    struct tm *ptr = localtime(&l_usedTime);
    strftime(buffer, sizeof(buffer), "Used Time: %S seconds", ptr);
    m_time->setString(buffer);
    
    sprintf(buffer, "Collected Enegy: %d", m_Layer->getPlayer()->getEatCount());
    m_enegy->setString(buffer);
    
    return true;
}