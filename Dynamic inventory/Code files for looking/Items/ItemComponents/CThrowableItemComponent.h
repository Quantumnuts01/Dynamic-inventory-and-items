#pragma once
#include "CBaseItemComponent.h"
#include <tgCSphere.h>
//enum EUpdateState
//{
//    STATE_SHOULD_NOT_UPDATE,
//    STATE_SHOULD_UPDATE
//};
class CBaseItem;
class CThrowableItemComponent :
    public CBaseItemComponent
{
public:
    CThrowableItemComponent(CBaseItem* pAttachedItem);
    ~CThrowableItemComponent();

    virtual void Use() override;

    virtual void Update(tgFloat DeltaTime);

private:
    bool Collision();
    tgCCollision*   m_pItemcollision;
    CBaseItem*      m_pAttachedItem;
    tgCWorld*       m_pWorld;
    tgCSphere       m_Sphere;
    bool            m_ShouldUpdate;
    tgCV3D          m_Dir;
    tgFloat         m_Speed;
    tgFloat         m_X;
};

