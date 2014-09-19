//
//  Menu_Complete.h
//  Tutorial1
//
//  Created by Apple on 14-9-18.
//
//

#ifndef MENU_COMPLETE_H
#define MENU_COMPLETE_H

#include "cocos2d.h"
#include "FishLayer.h"

NS_CC_BEGIN

class Menu_complete : public Menu
{
public:
    static Menu_complete *create(FishLayer *layer);
    //CREATE_FUNC(Menu_complete);
    
    bool init(FishLayer *layer);
protected:
    void reset();
    FishLayer *m_Layer;
};


NS_CC_END

#endif //MENU_COMPLETE_H