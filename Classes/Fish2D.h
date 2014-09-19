//
//  Fish2D.h
//  Tutorial1
//
//  Created by Apple on 14-8-15.
//
//

#ifndef FISH2D_H
#define FISH2D_H

#include "cocos2d.h"

#include <iostream>

#define MySprite Fish
#define Test2D

enum ActionType
{
    _AT_Start = 0,
    _AT_Swim,
    _AT_Hurt,
    _AT_End = 10000
};

enum MySpriteType
{
    _ST_Start   = 0,
    _ST_Scissor = 0x01,
    _ST_Hummer  = 0x02,
    _ST_Burden  = 0x04,
    _ST_Wall    = 0x08,
    _ST_Dest    = 0x10,
    _ST_End     = 0xff
};

class MySprite2DFactory;
class Fish : public cocos2d::Sprite
{
public:
    Fish() : m_Color(cocos2d::Color3B(255,255,255)) {}
    virtual bool initWithFile(std::string file);
    static Fish *create(Fish *fish);
    int getType() {return m_SpriteType;}
    virtual int getEnemy() {return 0;}
    virtual int getFood() {return 0;}
    
    virtual void setBodyContactMask();
protected:
    std::string         m_ModelFile;
    int                 m_SpriteType;
    cocos2d::Color3B    m_Color;
    cocos2d::Map<int, cocos2d::Action*>      m_Actions;


};



class Scissor : public Fish
{
    Scissor();
public:
    int getEnemy() {return _ST_Hummer;}
    int getFood() {return _ST_Burden;}
    
    friend class MySprite2DFactory;
};

class Hummer : public Fish
{
    Hummer();
public:
    int getEnemy() {return _ST_Burden;}
    int getFood() {return _ST_Scissor;}
    
    friend class MySprite2DFactory;
};

class Burden : public Fish
{
    Burden();
public:
    int getEnemy() {return _ST_Scissor;}
    int getFood() {return _ST_Hummer;}
    
    friend class MySprite2DFactory;
};

class Wall : public Fish
{
    Wall();
public:
    void setBodyContactMask();
    friend class MySprite2DFactory;
};

class Dest : public Fish
{
    Dest();
public:
    friend class MySprite2DFactory;
};

//对象工厂
class MySprite2DFactory
{
public:
    Fish *Create(int type);
    
};

#endif
