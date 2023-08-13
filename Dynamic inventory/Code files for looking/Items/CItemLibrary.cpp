
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
	CubeData._MaxStackSize = 10;
	CubeData._ModelPath = tgCString("models/box");
	CubeData._IconPath = tgCString("cube");
	//After the data struct is done you add it to the map containing all itemdata and use the 'GetNewID()' function to get an ID for the new data.
	m_ItemData.insert({ GetNewID(), CubeData});

	SItemData HelmetData;
	HelmetData._MaxStackSize = 20;
	HelmetData._IconPath = tgCString("mando_helm");
	HelmetData._ModelPath = tgCString("models/Mando_Helmet");
	m_ItemData.insert({ GetNewID(), HelmetData });

	SItemData	GunData;
	GunData._MaxStackSize = 1;
	GunData._IconPath = tgCString("gun");
	GunData._ModelPath = tgCString("models/dc-15s");
	GunData._IsEquipable = true;
	m_ItemData.insert({ GetNewID(), GunData });

}

CItemLibrary::~CItemLibrary()
{
	m_ItemData.clear();
}

CBaseItem* CItemLibrary::CreateItem(tgSInt32 ID)
{

	if (m_ItemData.find(ID) == m_ItemData.end())
	{
		return nullptr;
	}
	CBaseItem* pitem{};
	SItemData ItemData = m_ItemData.at(ID);
	switch (ID)
	{
	//This switch is what enables multiple classes to be created as items so when you make your own
	//just add a new case corresponding to the id of the itemdata for your item
	case 1:
	{
		pitem = new CDebugCube(ID, ItemData);
	}
	break;
	case 2:
	{
		pitem = new CHelmet(ID, ItemData);
	}
	break;
	case 3:
	{
		pitem = new CGun(ID, ItemData);
	}
	break;
	default: break;
	}
	return pitem;
}

tgBool CItemLibrary::DoesItemIDExist(tgSInt32 ID )
{
	return true; // ItemTypes.at(ID);
}

tgSInt32 CItemLibrary::GetNewID()
{
	return m_ItemData.size()+1;
}
