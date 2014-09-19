//
//  Menu_Complete.cpp
//  Tutorial1
//
//  Created by Apple on 14-9-18.
//
//

#include <stdio.h>
#include "Menu_Complete.h"

USING_NS_CC;

Menu_complete *Menu_complete::create(FishLayer *layer)
{
    auto menu = new Menu_complete();
    
    if(menu && menu->init(layer))
    {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE(menu);
    return nullptr;
}

bool Menu_complete::init(cocos2d::FishLayer *layer)
{
    auto menu = MenuItemImage::create("reset.png", "reset.png", CC_CALLBACK_0(Menu_complete::reset, this));
    menu->setPosition(Vec2::ZERO);
    
    m_Layer = layer;
    
    setAnchorPoint(Vec2(0.5, .5));
    
    setColor(Color3B(227,108,12));
    
    Vector<MenuItem*> items;
    items.pushBack(menu);
    
    Menu::initWithArray(items);
    
    return true;
}

void Menu_complete::reset()
{
    m_Layer->reset();
    removeFromParent();
}