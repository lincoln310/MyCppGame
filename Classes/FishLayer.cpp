
#include "FishLayer.h"
#include "MyAction.h"
#include "SelectItem.h"
#include "Menu_Complete.h"
#include <iostream>
#include <random>
#include <time.h>

USING_NS_CC;
using namespace std;

FishLayer *FishLayer::create()
{
	FishLayer *ret = new FishLayer();
	if(ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void FishLayer::reset()
{
    if(m_Player)
    {
        m_Player->removeFromParent();
        m_Player = nullptr;
    }
    for(MySprite *sp : m_Sprites)
    {
        sp->removeFromParent();
    }
    m_Sprites.clear();
    
    m_Status = _T_ST_Waiting;
    
    m_Direction = Vec2(0, 0);
    
    setMapLayer(m_MapLayer);
}

void FishLayer::start()
{
	m_StartTime = time(&m_StartTime);
	m_Status = _T_ST_Started;
}

void FishLayer::complete()
{
	m_CompleteTime = time(&m_CompleteTime);
	m_Status = _T_ST_Completed;
    
    auto l_LayerComplete = Layer_Complete::create(this);
    
    if(l_LayerComplete)
    {
        l_LayerComplete->setPosition(Director::getInstance()->getVisibleSize() / 2);
        addChild(l_LayerComplete);
    }
    
    if(m_Player)
    {
        m_Player->removeFromParent();
        m_Player = nullptr;
    }
}

void FishLayer::setPlayer(MySprite *sp)
{
    m_Player = sp;
    m_Player->setPosition(m_Orig);
    addChild(sp);
}

void FishLayer::setSpritePosition(MySprite *sp, Vec2 tileTag)
{
    //if(m_Walls.find(tileTag) == m_Walls.end())
    if(m_MapLayer->CheckAvilableByTag(tileTag))
    {
        auto nextPos = m_MapLayer->TileCoordToPositionMid(tileTag);
        auto act = MoveTo::create(1, nextPos);
        act->setTag(1);
        sp->runAction(act);
        //cout << " set pos" << endl;
        //this->setPlayerPosition(playerPos);
    }
}

void FishLayer::setMapLayer(MapLayer *mapLayer)
{
    if(m_MapLayer == nullptr)
        m_MapLayer = mapLayer;
    
    MySprite2DFactory spf;
    
    auto map = m_MapLayer->GetMap();
    
    auto sprites = map->getObjectGroup("Sprites")->getObjects();
    for(vector<Value>::const_iterator l_it = sprites.begin(); l_it != sprites.end(); ++l_it)
    {
        const ValueMap &vm = l_it->asValueMap();
        float x = vm.at("x").asFloat();
        float y = vm.at("y").asFloat();
        int t = vm.at("type").asInt();
        x += m_MapLayer->GetTileSize().width / 2;
        y += m_MapLayer->GetTileSize().height / 2;
        
        Vec2 tag = m_MapLayer->TileCoordForPosition(Vec2(x,y));
        Vec2 newpos = m_MapLayer->TileCoordToPositionMid(tag);
        
        if(t > 0)
        {
            MySprite *sp = spf.Create(t);
            sp->setPosition(newpos);
            addSprite(sp);
            addChild(sp);
        }
    }
    
    auto OrigDest = map->getObjectGroup("Spe_Place")->getObjects();
    for(vector<Value>::const_iterator l_it = OrigDest.begin(); l_it != OrigDest.end(); ++l_it)
    {
        const ValueMap &vm = l_it->asValueMap();
        float x = vm.at("x").asFloat();
        float y = vm.at("y").asFloat();
        string n = vm.at("name").asString();
        x += m_MapLayer->GetTileSize().width / 2;
        y += m_MapLayer->GetTileSize().height / 2;
        
        Vec2 tag = m_MapLayer->TileCoordForPosition(Vec2(x,y));
        Vec2 newpos = m_MapLayer->TileCoordToPositionMid(tag);
        
        if(n == "Orig")
            m_Orig = newpos;
        else if(n == "Dest")
        {
            m_Dest = newpos;
            if(m_DestSp == nullptr)
            {
                m_DestSp = spf.Create(_ST_Dest);
                m_DestSp->setPosition(m_Dest);
                addChild(m_DestSp);
            }
        }
    }
}

bool FishLayer::init()
{
    //addStaticItem(this);
    m_Status = _T_ST_Waiting;
    
    scheduleUpdate();
    
    //添加Touch监听
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);
    myListener->onTouchBegan = [=](Touch *touch, Event *event){return true;};
    myListener->onTouchMoved = CC_CALLBACK_2(FishLayer::onTouchMoved, this);
    myListener->onTouchEnded = CC_CALLBACK_2(FishLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
    //添加碰撞监听
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FishLayer::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

bool FishLayer::onContactBegin(const PhysicsContact& contact)
{
    MySprite *sp1 = (MySprite*)contact.getShapeA()->getBody()->getNode();
    MySprite *sp2 = (MySprite*)contact.getShapeB()->getBody()->getNode();
    
    
    
    MySprite *loser = nullptr;
    MySprite *winner = nullptr;
    if(sp1->getFood() == sp2->getType())
        loser = sp2;
    else
        loser = sp1;
    
    winner = loser == sp1 ? sp2 : sp1;
    
    CCLOG("onContactBegin, remove %d", loser->getType());
    
    if((loser ==  m_Player) || (sp1 == m_DestSp && sp2 == m_Player) || (sp2 == m_DestSp && sp1 == m_Player))
    {
        complete();
        return true;
    }
    
    
    winner->addEatCount(loser->getEatCount());
    removeSprite(loser);
    
    return true;
}

void FishLayer::onTouchMoved(Touch *touch, Event *event)
{
    auto delta = touch->getDelta();
    if(abs(delta.y) > abs(delta.x))
    {
        m_Direction = Vec2(0, - delta.y / abs(delta.y));
    }
    else
    {
        m_Direction = Vec2(delta.x / abs(delta.x), 0);
    }
}

void FishLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if(m_Status == _T_ST_Waiting)
    {
        int typeSt[3] = {1, 2, 4};
        static vector<int> types(&typeSt[0], &typeSt[3]);
        auto l_LayerSelect = SelectItemLayer::create(m_MapLayer->GetTileSize(), types);
        if(l_LayerSelect )
        {
            l_LayerSelect->setPosition(Director::getInstance()->getVisibleSize() / 2);
            addChild(l_LayerSelect);
        }
    }
}

void FishLayer::addSprite(MySprite *sp)
{
    m_Sprites.insert(sp);
}

void FishLayer::removeSprite(MySprite *sp)
{
    m_Sprites.erase(sp);
    sp->removeFromParent();
}

void FishLayer::update(float dt)
{
    
    if(m_Status != _T_ST_Started)
        return;
    
    //player update
    auto act = m_Player->getActionByTag(1);
    
    if(act == nullptr)
    {
        Vec2 curPos = m_Player->getPosition();
        auto curTag = m_MapLayer->TileCoordForPosition(curPos);
        
        setSpritePosition(m_Player, curTag + m_Direction);
    }
    
    //sprite update
    map<Vec2, MySprite*> allSpWithTag;
    set<int>        allTypesLeft;
    
    for(MySprite *sp : m_Sprites)
    {
        auto tag = m_MapLayer->TileCoordForPosition(sp->getPosition());
        allSpWithTag.insert(make_pair(tag, sp));
        allTypesLeft.insert(sp->getType());
    }
    
    Vec2 pos = m_Player->getPosition();
    Vec2 tag = m_MapLayer->TileCoordForPosition(pos);
    allSpWithTag.insert(make_pair(tag, m_Player));
    allTypesLeft.insert(m_Player->getType());

    for(std::map<Vec2, MySprite*>::iterator l_it = allSpWithTag.begin(); l_it != allSpWithTag.end(); ++l_it)
    {
        MySprite *sp = l_it->second;
        
        if(sp == m_Player)
            continue;
        
        if(allTypesLeft.find(sp->getFood()) == allTypesLeft.end())
            continue;
        
        auto act = sp->getActionByTag(1);
        
        if(act)
            continue;
        
        Vec2 foodPos = getNearestTarDir(l_it->first, sp->getFood(), sp->getEnemy(), allSpWithTag);
        setSpritePosition(sp, foodPos);
    }
}

Vec2 FishLayer::getNextPosition(MySprite *sp)
{
    Vec2 ret;
    
    const Vec2 &curPos = sp->getPosition();
    Vec2 curTag = m_MapLayer->TileCoordForPosition(curPos);
    
    float minDis = MAXFLOAT;
    int tarType = sp->getFood();
    
    for(std::set<MySprite*>::iterator l_it = m_Sprites.begin(); l_it != m_Sprites.end(); ++l_it)
    {
        MySprite *tmpSp = *l_it;
        
        if (tmpSp == sp)
            continue;
        
        if (tarType != tmpSp->getType())
            continue;
        
        const Vec2 &tmpPos = tmpSp->getPosition();
        Vec2 tmpTag = m_MapLayer->TileCoordForPosition(tmpPos);
        
        vector<Vec2> path = m_MapLayer->GetPathFromTagToTag(curTag, tmpTag);
        if(path.empty())
            continue;
        
        int dis = path.size();
        
        if(dis < minDis)
        {
            minDis = dis;
            ret = path.front();
        }
    }
    
    return ret;
}

Vec2 FishLayer::getNearestTarDir(cocos2d::Vec2 fromTag, int tarType, int enemyType, const map<Vec2, MySprite*> &allSps)
{
    Vec2 ret = fromTag;
    bool IsFood = false;
    
    PathNodePool l_Pool;
    
    typedef multimap<float, PathNode*> OpenSetType;
    
    PathNode *orig = l_Pool.create(fromTag);
    OpenSetType openSet;
    set<Vec2> closeSet;
    
    openSet.insert(make_pair(orig->cost, orig));
    
    while(openSet.empty() != true)
    {
        OpenSetType::iterator l_curIt = openSet.begin();
        PathNode *l_cur = l_curIt->second;
        
        map<Vec2, MySprite*>::const_iterator itFound = allSps.find(l_cur->curTag);
        if(itFound != allSps.end())
        {
            MySprite *sp = itFound->second;
            if(sp->getType() == tarType)
            {
                IsFood = true;
                break;
            }
            else if(sp->getType() == enemyType)
                break;
        }
        
        vector<Vec2> l_nexts = m_MapLayer->GetAligns(l_cur->curTag);
        
        for(vector<Vec2>::iterator l_it = l_nexts.begin(); l_it != l_nexts.end(); ++l_it)
        {
            Vec2 tmp = *l_it;
            if(l_cur->preNode && tmp == l_cur->preNode->curTag)
                continue;
            if(m_MapLayer->CheckAvilableByTag(tmp) != true)
                continue;
            if(closeSet.find(tmp) != closeSet.end())
                continue;
            PathNode *l_next = l_Pool.create(tmp, l_cur);
            l_next->cost = l_cur->cost + 1;
            
            openSet.insert(make_pair(l_next->cost, l_next));
            closeSet.insert(tmp);
        }
        
        openSet.erase(l_curIt);
        closeSet.insert(l_cur->curTag);
    }
    
    if(openSet.empty() != true)
    {
        PathNode *l_End = openSet.begin()->second;
        while(l_End->preNode != nullptr)
        {
            ret = l_End->curTag;
            l_End = l_End->preNode;
        }
    }
    
    Vec2 enemyDir = ret;
    //enemy is even near
    if(IsFood != true)
    {
        vector<Vec2> l_nexts = m_MapLayer->GetAligns(fromTag);
        vector<Vec2> l_Available;
        for(Vec2 v : l_nexts)
        {
            if(v == enemyDir)
                continue;
            if(m_MapLayer->CheckAvilableByTag(v) != true)
                continue;
            l_Available.push_back(v);
        }
        
        if(l_Available.size() == 0)
            l_Available.push_back(enemyDir);
        
        uniform_int_distribution<unsigned> u(0,l_Available.size() - 1);//生成0到size（包含）均匀分布的随机数
        default_random_engine e;//生成随机无符号数
        int l_r = u(e);
        ret = l_Available[l_r];
    }
    return ret;
}

