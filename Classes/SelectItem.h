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

    static std::string getImg(int type);
    
    int ItemType;
    Vec2 ItemPos;
};

class SelectItemLayer : public Menu
{
public:
    static SelectItemLayer *create(const Size &tagSize, const std::vector<int> &types, const Vec2 &spPoi);
    //static SelectItemLayer *createWithArray(const Vector<MenuItem*>& arrayOfItems);
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems, const Vec2 &poi);
    
    void onTouchEnded(Touch *touch, Event *event);
    
    
protected:
    FishLayer *m_FishLayer;
    std::vector<SelectItem*> m_Items;
    Size        m_TagSize;
    Vec2        m_SpPoi;
};


NS_CC_END
#endif //_SELECTITEM_H