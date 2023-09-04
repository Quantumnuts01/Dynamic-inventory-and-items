
#include	<tgSystem.h>
#include	"CItemLibrary.h"
#include	"CBaseItem.h"
#include	"ItemComponents/CThrowableItemComponent.h"
#include	<tgCV3D.h>
#include	"CDebugCube.h"
#include	"CHelmet.h"
#include	"CGun.h"

CItemLibrary::CItemLibrary()
{


	//To add a new item you first create the item data, this contains the Max Stack Size, model path and icon path.
	SItemData	CubeData;
	CubeData._ItemID = 1;
	CubeData._MaxStackSize = 10;
	CubeData._ModelPath = tgCString("models/box");
	CubeData._IconPath = tgCString("cube");
	//After the data struct is done you add it to the map containing all itemdata and use the 'GetNewID()' function to get an ID for the new data.
	m_ItemData.insert({ typeid(CDebugCube), CubeData});

	SItemData HelmetData;
	HelmetData._ItemID = 2;
	HelmetData._MaxStackSize = 20;
	HelmetData._IconPath = tgCString("mando_helm");
	HelmetData._ModelPath = tgCString("models/Mando_Helmet");
	HelmetData._IsEquipable = true;
	m_ItemData.insert({ typeid(CHelmet), HelmetData});

	SItemData	GunData;
	GunData._ItemID = 3;
	GunData._MaxStackSize = 1;
	GunData._IconPath = tgCString("gun");
	GunData._ModelPath = tgCString("models/dc-15s");
	GunData._IsEquipable = true;
	m_ItemData.insert({ typeid(CGun), GunData});

}

CItemLibrary::~CItemLibrary()
{
	m_ItemData.clear();
}

tgBool CItemLibrary::DoesItemIDExist(tgSInt32 /*ID*/ )
{
	return true; // ItemTypes.at(ID);
}

tgSInt32 CItemLibrary::GetNewID()
{
	return m_ItemData.size()+1;
}
