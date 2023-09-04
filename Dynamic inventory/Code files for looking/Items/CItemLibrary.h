#pragma once

#include	<tgMemoryDisable.h>
#include	<iostream>
#include	<map>
#include	<typeinfo>
#include	<typeindex>
#include	<tgMemoryEnable.h>

#include	"CBaseItem.h"
#include	<tgCSingleton.h>


class CItemLibrary : public tgCSingleton< CItemLibrary >
{

public:
	CItemLibrary();
	~CItemLibrary();
	template<typename T >
	T* CreateItem();
	tgBool DoesItemIDExist(tgSInt32 ID);

private:
	tgSInt32 GetNewID();
	std::map< std::type_index , SItemData > m_ItemData;

};

template<typename T>
inline T* CItemLibrary::CreateItem()
{
	if (m_ItemData.find(typeid(T)) == m_ItemData.end())
	{
		return nullptr;
	}
	T* pitem;
	SItemData ItemData = m_ItemData.at(typeid(T));
	pitem = new T(ItemData._ItemID, ItemData);
	
	return pitem;
}

