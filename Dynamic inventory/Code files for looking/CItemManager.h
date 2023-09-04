#pragma once

#include <tgCSingleton.h>

#include	<tgMemoryDisable.h>
#include	<map>
#include	<tgMemoryEnable.h>
class CBaseItem;

class CItemManager : public tgCSingleton< CItemManager >
{
public:

	CItemManager( void );
	~CItemManager( void );

//////////////////////////////////////////////////////////////////////////
	template<typename T>
	void CreateItemInWorld(tgCV3D Position);

	void AddItemToWorld( CBaseItem* Item, tgCV3D Position );

	void DestroyItemInWorld( tgCString ItemName );

	void UpdateItemsInWorld( tgFloat DeltaTime );

	CBaseItem* GetItemFromWorld(tgCString ItemName);

	CBaseItem* LineItemsInWorld( tgCLine3D Line, tgCCollision* pItemCollision );

	std::map <tgCString, CBaseItem*>& GetItemMap( void ) { return m_pItemsInWorld; }

//////////////////////////////////////////////////////////////////////////
private:

	std::map <tgCString, CBaseItem*> m_pItemsInWorld;

};

template<typename T>
void CItemManager::CreateItemInWorld(tgCV3D Position)
{
	T* Temp = CItemLibrary::GetInstance().CreateItem<T>();
	Temp->SpawnItemModel(Position);
	m_pItemsInWorld.insert({ Temp->GetItemName(), Temp });

}
