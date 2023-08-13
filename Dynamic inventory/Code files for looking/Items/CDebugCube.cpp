#include <tgSystem.h>
#include "CDebugCube.h"

CDebugCube::CDebugCube(tgSInt32 ItemID, SItemData ItemData) : CBaseItem(ItemID, ItemData)
{
}

CDebugCube::~CDebugCube()
{
}

tgBool CDebugCube::UseItem()
{
	return tgBool();
}
