#pragma once

#include	<tgCString.h>
#include	"ItemComponents/CBaseItemComponent.h"
#include	<tgMemoryDisable.h>
#include	<iostream>
#include	<map>
#include	<sstream>
#include	<string>
#include	<vector>
#include	<tgMemoryEnable.h>


class CPlayer;
class CBaseItemComponent;

struct SItemData
{
	tgSInt32	_ItemID;
	tgCString	_IconPath;
	tgSInt32	_MaxStackSize;
	tgCString	_ModelPath;
	tgSInt32	_StackSize = 1;
	tgBool		_IsEquipable = false;
};
class CBaseItem
{
protected:


public:
	// Constructor / Destructor
	CBaseItem(tgSInt32 ID, SItemData Data);
	~CBaseItem(void);

	//////////////////////////////////////////////////////////////////////////

	virtual void	Update(const tgFloat Deltatime);
	virtual tgBool	UseItem(CPlayer* Owner) = 0;

	//////////////////////////////////////////////////////////////////////////
	
	virtual void	ItemSelected(CPlayer* Owner);
	

	//////////////////////////////////////////////////////////////////////////

	void			SetItemPosition(tgCV3D Position);
	void			SetItemStackSize(tgSInt32 NrOfItems) { m_Data._StackSize = NrOfItems; }

	//////////////////////////////////////////////////////////////////////////

	tgSInt32&		GetItemID() { return m_ID; }
	SItemData&		GetItemData() { return m_Data; }
	tgCString&		GetItemName() { return m_ItemName; }
	tgCModel*		GetItemModel() { return m_pItemModel; }
	tgBool			GetIsEquipable() { return m_Data._IsEquipable; }

	//////////////////////////////////////////////////////////////////////////

	void			SpawnItemModel(tgCV3D SpawnPos);
	void			DespawnItemModel();

	//////////////////////////////////////////////////////////////////////////
protected:

	tgSInt32							m_ID;
	SItemData							m_Data;
	tgCString							m_ItemName;
	tgCModel*							m_pItemModel;
	std::vector<CBaseItemComponent*>	m_pComponents;
};

