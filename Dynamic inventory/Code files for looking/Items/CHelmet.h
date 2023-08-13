#pragma once
#include "CBaseItem.h"
class CHelmet :
    public CBaseItem
{
public:
    CHelmet(tgSInt32 ItemID, SItemData ItemData);
    ~CHelmet();
    virtual tgBool UseItem() override;
};

