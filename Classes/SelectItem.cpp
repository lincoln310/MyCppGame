//
//  SelectItem.cpp
//  Tutorial1
//
//  Created by Apple on 14-9-12.
//
//

#include <stdio.h>
#include "SelectItem.h"
#include "Fish2D.h"

USING_NS_CC;
using namespace std;

SelectItem::SelectItem(int type) : ItemType(type) {}

SelectItem *SelectItem::create(int type)
{
    auto ret = new SelectItem(type);
    if(ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SelectItem::init()
{
    string ItemImgName = SelectItem::getImg(ItemType);
    MenuItemImage::initWithNormalImage(ItemImgName, ItemImgName, ItemImgName, CC_CALLBACK_0(SelectItem::onTouchEnded, this));

    return true;
}

string SelectItem::getImg(int type)
{
    string ret;
    switch (type) {
        case _ST_Scissor:
            ret = "scissor.png";
            break;
        case _ST_Hummer:
            ret = "hammer.png";
            break;
        case _ST_Burden:
            ret = "burden.png";
            break;
        case _ST_Wall:
            ret = "wall.png";
            break;
        default:
            break;
    }
    return ret;
}

void SelectItem::onTouchEnded()
{
    static MySprite2DFactory spf;
    MySprite *sp = spf.Create(ItemType);
    if(sp)
    {
        sp->setColor(Color3B(239,239,239));
        auto event = EventCustom("select_sprite");
        event.setUserData(sp);
        _eventDispatcher->dispatchEvent(&event);
    }
}

SelectItemLayer *SelectItemLayer::create(const Size &tagSize, const std::vector<int> &types)
{
    SelectItemLayer *layer = new SelectItemLayer();
    if(layer && layer->initWithArray(tagSize, types))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool SelectItemLayer::initWithArray(const Size &tagSize, const std::vector<int> &types)
{
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    //添加背景
    auto visiableSize = Director::getInstance()->getVisibleSize();
    auto background = LayerColor::create(Color4B(128, 128, 128, 128), visiableSize.width / 2, visiableSize.height / 2);
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setPosition( -1 * visiableSize/4);
    addChild(background, -1);
    
    auto l_eventListener = EventListenerTouchOneByOne::create();
    l_eventListener->onTouchEnded = CC_CALLBACK_2(SelectItemLayer::onTouchEnded, this);
    l_eventListener->onTouchBegan = [=](Touch *touch, Event *event) {return true;};
    l_eventListener->onTouchMoved = [=](Touch *touch, Event *event) {};
    l_eventListener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(l_eventListener, this);
    
    
    //玩家选择精灵事件
    auto selectItemListener = EventListenerCustom::create("select_sprite", CC_CALLBACK_1(SelectItemLayer::onSelectSpriteEvent, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(selectItemListener, this);
    
    
    Vector<MenuItem*> l_Items;
    //添加可选项
    float scale = 3;
    int cnt = types.size();
    for(int i=0; i<types.size(); i++)
    {
        int tmpType = types[i];
        
        auto Img = SelectItem::create(tmpType);
        Img->setScale(scale);
        float tagx = i - (cnt - 1) / 2.0f;
        Vec2 tmpPos = Vec2(tagx * tagSize.width * scale, 0);
        Img->setPosition(tmpPos);
        l_Items.pushBack(Img);
    }
    
    auto l_menu = Menu::createWithArray(l_Items);
    l_menu->setAnchorPoint(Vec2(.5, .5));
    l_menu->setPosition(Vec2::ZERO);
    addChild(l_menu);
    
    return true;
}

void SelectItemLayer::setPlayer(MySprite *sp)
{
    FishLayer *parent = (FishLayer*)getParent();
    parent->setPlayer(sp);
    parent->start();
    removeFromParent();
}

void SelectItemLayer::onSelectSpriteEvent(EventCustom *event)
{
    std::string str("Custom event 1 received, ");
    MySprite* sp = static_cast<MySprite*>(event->getUserData());
    setPlayer(sp);
}

void SelectItemLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    removeFromParent();
}
