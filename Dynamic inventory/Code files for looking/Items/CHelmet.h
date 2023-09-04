#pragma once
#include "CBaseItem.h"
class CHelmet :
    public CBaseItem
{
public:
    CHelmet(tgSInt32 ItemID, SItemData ItemData);
    ~CHelmet();
    virtual tgBool UseItem(CPlayer* Owner) override;
    virtual void ItemSelected(CPlayer* Owner) override;
};

