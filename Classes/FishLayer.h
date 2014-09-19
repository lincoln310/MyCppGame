#ifndef _FISHLAYER_H
#define _FISHLAYER_H
#include "cocos2d.h"
#include "Math/Vec2.h"

#include "Fish2D.h"
//#include "Fish3D.h"

#include "MapLayer.h"

NS_CC_BEGIN

enum STATUS
{
    _T_ST_Waiting   = 0,
    _T_ST_Started   = 1,
    _T_ST_Completed = 2,
    _T_ST_Reset     = 3,
    _T_ST_GameOver  = 4
};


//这是用于显示3d鱼模型的层
class FishLayer : public  Layer
{
public:
	//初始化当前层
    virtual bool init();  
    static FishLayer *create();
    //CREATE_FUNC(FishLayer);

    void update(float dt);
    
    void setPlayer(MySprite *sp) {m_Player = sp; addChild(sp);}
    
    void addSprite(MySprite *sp);
    
    void removeSprite(MySprite *sp);
    
    void setMapLayer(MapLayer *mapLayer);
    
    void setSpritePosition(MySprite *sp, cocos2d::Vec2 pos);
    
    void setPhyWorld(PhysicsWorld *world) {m_world = world;}
    
    void reset();
protected:
    cocos2d::Vec2 getNextPosition(MySprite *sp);
    
    Vec2 getNearestTarDir(Vec2 fromTag, int foodType, int enemyType, const std::map<Vec2, MySprite*> &allSprites);
    
    bool onContactBegin(const PhysicsContact& contact);
    
    void onTouchEnded(Touch *touch, Event *event);
    
    void onTouchMoved(Touch *touch, Event *event);

    MapLayer                        *m_MapLayer;
    
    MySprite                        *m_Player;
	//鱼精灵
    std::set<MySprite*>             m_Sprites;
    Vec2                            m_Orig;
    Vec2                            m_Dest;
    MySprite                        *m_DestSp;
    
    std::map<Vec2, MySprite*>       m_Walls;
    
    PhysicsWorld                    *m_world;
    
    int                             m_Started;
    
    Vec2                            m_Direction;
};

NS_CC_END

#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
