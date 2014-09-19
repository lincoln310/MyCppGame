#include "MapLayer.h"
#include <iostream>
#include <map>
#include <set>

using namespace std;
using namespace cocos2d;

//MapLayer *MapLayer::create() {
//	MapLayer *ret = new MapLayer();
//	if (ret && ret->init()) {
//		ret->autorelease();
//		return ret;
//	}
//	CC_SAFE_DELETE(ret);
//	return nullptr;
//}

bool MapLayer::init()
{
    m_tmxMap = cocos2d::TMXTiledMap::create("TileMap.tmx");
    
    
    //m_tmxMap->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(m_tmxMap);
    
    m_tileSize = m_tmxMap->getTileSize();
    m_mapSize = m_tmxMap->getMapSize();
    
    m_WallLayer = m_tmxMap->getLayer("WallLayer");
    
    for(int x = 0; x < GetMapSize().width; x++)
    {
        for(int y = 0; y < GetMapSize().height; y++)
        {
            Vec2 tag = Vec2(x,y);
            int tileGid = m_WallLayer->getTileGIDAt(tag);
            auto colledable = m_WallLayer->getProperty("Collidable");
            if(tileGid && colledable.asBool())
                m_WallSet.insert(tag);
        }
    }
    return true;
}

Vec2 MapLayer::TileCoordForPosition(const cocos2d::Vec2 &pos)
{
    auto newpos = CC_POINT_POINTS_TO_PIXELS(pos);
    //cout << "Map Pos:" << newpos.x << " " << newpos.y << endl;
    int x = newpos.x / m_tileSize.width;
    int y = ((m_mapSize.height * m_tileSize.height) - newpos.y) / m_tileSize.height;
    
    return Vec2(x, y);
}

Vec2 MapLayer::TileCoordToPosition(const cocos2d::Vec2 &A_Tag)
{
    float x = A_Tag.x * m_tileSize.width;
    float y = (m_mapSize.height - A_Tag.y) * m_tileSize.height;
    
    return Vec2(x,y);
}

Vec2 MapLayer::TileCoordToPositionMid(const cocos2d::Vec2 &A_Tag)
{
    Vec2 ret = TileCoordToPosition(A_Tag);
    ret.x += m_tileSize.width / 2;
    ret.y -= m_tileSize.height / 2;
    return ret;
}

bool MapLayer::CheckAvilable(cocos2d::Vec2 pos)
{
    Vec2 tileCoord = TileCoordForPosition(pos);
    return CheckAvilableByTag(tileCoord);
}

bool MapLayer::CheckAvilableByTag(Vec2 tag)
{
    if(tag.x < m_mapSize.width &&
       tag.y < m_mapSize.height &&
       tag.x >= 0 &&
       tag.y >= 0)
    {
        if(m_WallSet.find(tag) != m_WallSet.end())
            return false;
    }
    else
        return false;
    return true;
}

std::vector<Vec2> MapLayer::GetAligns(cocos2d::Vec2 tag)
{
    vector<Vec2> ret;
    if(tag.x > 0)
        ret.push_back(Vec2(tag.x - 1, tag.y));
    if(tag.y > 0)
        ret.push_back(Vec2(tag.x, tag.y - 1));
    if(tag.x < m_mapSize.width)
        ret.push_back(Vec2(tag.x + 1, tag.y));
    if(tag.y < m_mapSize.height)
        ret.push_back(Vec2(tag.x, tag.y + 1));
    
    return ret;
}

std::vector<Vec2> MapLayer::GetPathFromTagToTag(cocos2d::Vec2 from, cocos2d::Vec2 to)
{
    vector<Vec2> ret;
    
    PathNodePool l_Pool;
    
    typedef multimap<float, PathNode*> OpenSetType;
    
    PathNode *orig = l_Pool.create(from);
    OpenSetType openSet;
    set<Vec2> closeSet;
    
    openSet.insert(make_pair(orig->cost, orig));
    
    while(openSet.empty() != true)
    {
        OpenSetType::iterator l_curIt = openSet.begin();
        PathNode *l_cur = l_curIt->second;
        if(to == l_cur->curTag)
            break;
        
        vector<Vec2> l_nexts = GetAligns(l_cur->curTag);
        
        for(vector<Vec2>::iterator l_it = l_nexts.begin(); l_it != l_nexts.end(); ++l_it)
        {
            Vec2 tmp = *l_it;
            if(l_cur->preNode && tmp == l_cur->preNode->curTag)
                continue;
            if(CheckAvilableByTag(tmp) != true)
                continue;
            if(closeSet.find(tmp) != closeSet.end())
                continue;
            PathNode *l_next = l_Pool.create(tmp, l_cur);
            l_next->cost = l_cur->cost + 1;
            
            openSet.insert(make_pair(l_next->cost, l_next));
        }
        
        openSet.erase(l_curIt);
        closeSet.insert(l_cur->curTag);
    }
    
    if(openSet.empty())
        return ret;
    
    PathNode *l_End = openSet.begin()->second;
    while(l_End->preNode != nullptr)
    {
        ret.push_back(l_End->curTag);
        l_End = l_End->preNode;
    }
    
    reverse(ret.begin(), ret.end());
    
    return ret;
}
