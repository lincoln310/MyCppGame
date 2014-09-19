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

SelectItemLayer *SelectItemLayer::create(const Size &tagSize, const std::vector<int> &types, const Vec2 &spPoi)
{
    float scale = 3;
    Vector<MenuItem*> Items;
    int cnt = types.size();
    for(int i=0; i<types.size(); i++)
    {
        int tmpType = types[i];
        
        string ItemImgName = SelectItem::getImg(tmpType);
        auto Img = SelectItem::create(ItemImgName, ItemImgName);
        Img->setTag(tmpType);
        Img->setScale(scale);
        float tagx = i - (cnt - 1) / 2.0f;
        Vec2 tmpPos = Vec2(tagx * tagSize.width * scale, 0);
        
        Img->setPosition(tmpPos);
        
        Items.pushBack(Img);
    }
    

    SelectItemLayer *layer = new SelectItemLayer();
    if(layer && layer->initWithArray(Items, spPoi))
    {
        layer->autorelease();
        return layer;
    }else
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void SelectItemLayer::onTouchEnded(Touch *touch, Event *event)
{
    SelectItem *item = (SelectItem*)getItemForTouch(touch);
    if(item)
    {
        static MySprite2DFactory spf;
        MySprite *sp = spf.Create(item->getTag());
        if(sp)
        {
            sp->setPosition(m_SpPoi);
            sp->setColor(Color3B(239,239,239));
            FishLayer *parent = (FishLayer*)getParent();
            parent->setPlayer(sp);
        }
    }
    removeFromParent();
}

bool SelectItemLayer::initWithArray(const Vector<MenuItem*>& arrayOfItems, const Vec2 &poi)
{
    Menu::initWithArray(arrayOfItems);
    
    setContentSize(Size(Director::getInstance()->getVisibleSize() / 2));
    
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    setColor(Color3B(239,239,239));
    
    m_SpPoi = poi;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [=](Touch *touch, Event *event) {return true;};
    listener->onTouchMoved = [=](Touch *touch, Event *event) {};
    listener->onTouchEnded = CC_CALLBACK_2(SelectItemLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}