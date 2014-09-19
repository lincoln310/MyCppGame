//
//  map.h
//  Tutorial1
//
//  Created by Apple on 14-8-24.
//
//

#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "cocos2d.h"
#include <string>
#include <vector>
#include <list>
#include <set>

NS_CC_BEGIN

class PathNode
{
public:
    PathNode(Vec2 tag, PathNode *pre = nullptr) : curTag(tag), preNode(pre), cost(0), cnt(0)
    {
        if(preNode)
            cnt = preNode->cnt + 1;
    }
    
    Vec2 curTag;
    PathNode *preNode;
    float cost;
    int cnt;
};

class PathNodePool
{
public:
    ~PathNodePool()
    {
        for(std::list<PathNode*>::iterator l_it = m_allNodes.begin(); l_it != m_allNodes.end(); ++l_it)
            delete *l_it;
    }
    PathNode *create(Vec2 &tag, PathNode *pre = nullptr)
    {
        PathNode *ret = new PathNode(tag, pre);
        m_allNodes.push_back(ret);
        return ret;
    }
    
private:
    std::list<PathNode*> m_allNodes;
};

class MapLayer : public Layer
{
public:
    CREATE_FUNC(MapLayer);
    //static MapLayer *create();
    //构建函数
    virtual bool init();
    
    //获取特定坐标的索引
    Vec2 TileCoordForPosition(const Vec2 &A_point);
    
    //获取索引的坐标
    Vec2 TileCoordToPosition(const Vec2 &A_Tag);
    
    Vec2 TileCoordToPositionMid(const Vec2 &A_Tag);
    
    //获取特定Tile的坐标
    int GetTilePoi(int A_Idx);
    
    //根据坐标获取z等级
    int GetLevelZ(Vec2 &A_point);
    
    //根据索引获取z等级
    int GetLevelZ(int A_Idx);
    
    //检查索引处是否可用
    bool CheckAvilableByTag(Vec2 tag);
    
    //检查某个位置是否可用（没有障碍）
    bool CheckAvilable(Vec2 pos);
    
    std::vector<Vec2> GetPathFromTagToTag(Vec2 from, Vec2 to);
    
    //获取某个Tile的临近Tile
    std::vector<Vec2> GetAligns(Vec2 tag);
    
    Size GetTileSize() {return m_tileSize;}
    Size GetMapSize() {return m_mapSize;}
    
    TMXTiledMap     *GetMap() {return m_tmxMap;}
    TMXLayer        *GetWallLayer() {return m_WallLayer;} //m_tmxMap->getLayer("Walls");}
protected:
    Size m_tileSize;
    Size m_mapSize;
    
    std::set<Vec2>  m_Walls;
    
    TMXLayer       *m_WallLayer;
    TMXTiledMap    *m_tmxMap;
    
    std::set<Vec2>  m_WallSet;
};

NS_CC_END

#endif //TMXTiledMapH
