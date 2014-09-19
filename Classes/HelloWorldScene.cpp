#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    PhysicsWorld *world = scene->getPhysicsWorld();
    world->setGravity(Vec2::ZERO);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    PhysicsBody *Bound = PhysicsBody::createEdgeBox(visibleSize);
    Bound->setCategoryBitmask(_ST_Wall);
    Bound->setCollisionBitmask(_ST_End);
    auto edgeNode = Node::create();
    edgeNode->setPosition(visibleSize/2);
    edgeNode->setPhysicsBody(Bound);
    scene->addChild(edgeNode);
    // 'layer' is an autorelease object
    HelloWorld*		layer = HelloWorld::create();
    layer->setPhyWorld(scene->getPhysicsWorld());

    

    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setScale(2);
    closeItem->setAnchorPoint(Vec2(1,1));
    closeItem->setPosition(visibleSize);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    m_FishLayer = FishLayer::create();
    addChild(m_FishLayer);
    
    m_MapLayer = MapLayer::create();
    addChild(m_MapLayer, -1);
    
    if(m_FishLayer)
        m_FishLayer->setMapLayer(m_MapLayer);
    return true;
}



void HelloWorld::setPhyWorld(cocos2d::PhysicsWorld *world)
{
    m_world = world;
    if(m_FishLayer)
        m_FishLayer->setPhyWorld(getPhyWorld());
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
