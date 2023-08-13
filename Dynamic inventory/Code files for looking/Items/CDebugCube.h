#pragma once
#include "CBaseItem.h"
class CDebugCube :
    public CBaseItem
{
public:
    CDebugCube(tgSInt32 ItemID, SItemData ItemData);
    ~CDebugCube();
    virtual tgBool UseItem() override;
};

