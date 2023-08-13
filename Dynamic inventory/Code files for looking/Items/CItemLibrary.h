#pragma once

#include	<tgMemoryDisable.h>
#include	<iostream>
#include	<map>
#include	<tgMemoryEnable.h>

#include	"CBaseItem.h"
#include	<tgCSingleton.h>


class CItemLibrary : public tgCSingleton< CItemLibrary >
{

public:
	CItemLibrary();
	~CItemLibrary();
	CBaseItem* CreateItem(tgSInt32 ID);
	tgBool DoesItemIDExist( tgSInt32 ID );

private:
	tgSInt32 GetNewID();
	std::map<tgSInt32, SItemData > m_ItemData;

};

