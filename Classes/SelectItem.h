//
//  SelectItem.h
//  Tutorial1
//
//  Created by Apple on 14-9-12.
//
//

#ifndef _SELECTITEM_H
#define _SELECTITEM_H

#include "cocos2d.h"
#include "FishLayer.h"

#include <list>
#include <vector>

NS_CC_BEGIN

class SelectItem : public MenuItemImage
{
public:
    SelectItem(int type);
    
    static SelectItem *create(int type);
    
    bool init();

    static std::string getImg(int type);
    
    void onTouchEnded();
    
    int ItemType;
};

class SelectItemLayer : public Layer
{
public:
    static SelectItemLayer *create(const Size &tagSize, const std::vector<int> &types);
    //static SelectItemLayer *createWithArray(const Vector<MenuItem*>& arrayOfItems);
    bool initWithArray(const Size &tagSize, const std::vector<int> &types);
    
    void setPlayer(MySprite *sp);
    
    void onTouchEnded(Touch *touch, Event *event);
    
    void onSelectSpriteEvent(EventCustom *event);
};


NS_CC_END
#endif //_SELECTITEM_H