#include "Fish3D.h"

USING_NS_CC;

MySprite3DFactory::MySprite3DFactory()
{
    m_ModelFilesOfType[MySpriteType::_FT_Scissors] = "scissor";
    m_ModelFilesOfType[MySpriteType::_FT_Hammer] = "hammer";
    m_ModelFilesOfType[MySpriteType::_FT_Burden] = "burden";
}

Sprite3D *MySprite3DFactory::create(cocos2d::MySpriteType type)
{
    
    Sprite3D *sp = Sprite3D::create(m_ModelFilesOfType[type]);
    if(sp)
    {
        sp->setAnchorPoint(Vec2(0, 1));
        sp->runAction(getAction(type));
    }
    return sp;
}
Action *MySprite3DFactory::getAction(cocos2d::MySpriteType type)
{
    std::string file = m_ModelFilesOfType[type];
    
    auto animation = Animation3D::create(file);
    if (animation)
    {
        Action *act = nullptr;
        //act = RepeatForever::create(Sequence::create(Swim, NULL));
        return act;
    }
    return nullptr;
}
