
#include <tgSystem.h>
#include "CItemManager.h"
#include "CModelManager.h"
#include "GameStateMachine/CGameStates.h"
#include "../Specialization/Items/CBaseItem.h"
#include "../Specialization/Items/CItemLibrary.h"
#include <tgCString.h>
#include <tgCLine3D.h>
#include <tgCCollision.h>
#include <tgCMesh.h>
CItemManager::CItemManager(void)
{
	
}


CItemManager::~CItemManager(void)
{
	std::map <tgCString,CBaseItem*> Temp = m_pItemsInWorld;
	m_pItemsInWorld.clear();
	for (auto i : Temp)
	{
		delete i.second;
	}
}

void CItemManager::CreateItemInWorld( tgSInt32 ItemID, tgCV3D Position)
{
	CBaseItem* Temp = CItemLibrary::GetInstance().CreateItem( ItemID );
	Temp->SpawnItemModel(Position);
	m_pItemsInWorld.insert({Temp->GetItemName(), Temp});

}

void CItemManager::AddItemToWorld(CBaseItem* Item, tgCV3D Position)
{
	Item->SpawnItemModel(Position);
	m_pItemsInWorld.insert({Item->GetItemName(), Item });
}

void CItemManager::DestroyItemInWorld( tgCString ItemName )
{
	delete m_pItemsInWorld[ ItemName ];
	m_pItemsInWorld.erase( ItemName );
}

CBaseItem* CItemManager::GetItemFromWorld( tgCString ItemName)
{
	if (m_pItemsInWorld.find(ItemName) != m_pItemsInWorld.end())
	{
		CBaseItem* pRetItem = m_pItemsInWorld[ItemName];
		m_pItemsInWorld.erase(ItemName);
		return pRetItem;
	}

	return nullptr;
}

CBaseItem* CItemManager::LineItemsInWorld( tgCLine3D Line, tgCCollision* pItemCollision)
{

	//We try the collision against all items in the world. since tengine is not great at handeling objects

	for (auto Item : m_pItemsInWorld)
	{
		//the collision has to be cleared every time since all data from the linetrace is stored within that object
		pItemCollision->Clear();
		pItemCollision->SetType(tgCMesh::EType::TYPE_MODEL);
		if (pItemCollision->LineAllMeshesInModel(Line, *Item.second->GetItemModel()))
		{
			return Item.second;
		}
	}
	return nullptr;
}

void CItemManager::UpdateItemsInWorld( tgFloat DeltaTime )
{
	
}



