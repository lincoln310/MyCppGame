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

class Layer_Complete : public Layer
{
public:
    static Layer_Complete *create(FishLayer *layer);
    
    bool init(FishLayer *layer);
    
    void reset();
protected:
    FishLayer *m_Layer;
    LabelAtlas  *m_time;
    LabelAtlas  *m_enegy;
};


NS_CC_END

#endif //MENU_COMPLETE_H