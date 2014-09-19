//
//  Fish3D.h
//  Tutorial1
//
//  Created by Apple on 14-8-13.
//
//

#ifndef Tutorial1_Fish_h
#define Tutorial1_Fish_h

#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "3d/CCMesh.h"
#include "Math/Vec2.h"
// Include any system framework and library headers here that should be included in all compilation units.
// You will also need to set the Prefix Header build setting of one or more of your targets to reference this file.

NS_CC_BEGIN

#define Test3D
#define MySprite Sprite3D

enum MySpriteType
{
    _FT_Start = 0,
    _FT_Scissors = 1,
    _FT_Burden = 2,
    _FT_Hammer = 3,
    _FT_End = 10000
};

std::map<int, std::string> MySpriteFile;

class MySprite3DFactory
{
public:
    
    MySprite3DFactory();
    
    MySprite *create(MySpriteType type);
protected:
    Action *getAction(MySpriteType type);
    
    std::map<MySpriteType, std::string> m_ModelFilesOfType;
};

NS_CC_END

#endif
