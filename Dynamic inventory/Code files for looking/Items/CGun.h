#pragma once
#include    "CBaseItem.h"

class CGun :
    public CBaseItem
{
public:
    CGun(tgSInt32 ItemID, SItemData ItemData);
    ~CGun();
    virtual tgBool UseItem() override;
    virtual void	ItemSelected(CPlayer* Owner) override;

    virtual void Update(const tgFloat Deltatime) override;
    void Shoot();

private:
};

