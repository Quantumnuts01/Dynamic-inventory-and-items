
#include <tgSystem.h>
#include "CHelmet.h"

CHelmet::CHelmet(tgSInt32 ItemID, SItemData ItemData) :CBaseItem(ItemID,ItemData)
{
}
CHelmet::~CHelmet()
{
}

tgBool CHelmet::UseItem()
{
	return tgBool();
}
