#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "FishLayer.h"
#include "MapLayer.h"

NS_CC_BEGIN 
class HelloWorld : public cocos2d::Layer
{
	
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    void setPhyWorld(PhysicsWorld *world);
    PhysicsWorld *getPhyWorld() {return m_world;}
    
    // a selector callback
    void menuCloseCallback(Ref* sender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    PhysicsWorld                *m_world;
    //����Ĳ�
    FishLayer					*m_FishLayer;
    MapLayer					*m_MapLayer;
};
NS_CC_END
#endif // __HELLOWORLD_SCENE_H__
