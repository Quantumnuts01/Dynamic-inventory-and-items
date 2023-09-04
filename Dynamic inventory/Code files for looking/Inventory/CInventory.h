#pragma once

#include	<tgMemoryDisable.h>
#include	<imgui.h>
#include	<vector>
#include	<map>
#include	<iostream>
#include	<cstdlib>
#include	<tgMemoryEnable.h>
#include	"../Inventory/CInventorySlot.h"
#include	<tgCInputListener.h>
#include	<tgCLine3D.h>

enum EMenuState
{
	STATE_MAIN_MENU,
	STATE_SLOT_MENU
};	// EMenuState


class CPlayer;
class CBaseItem;


class CInventory : public tgCInputListener
{
public:
	struct SDragDropPayload
	{
		CBaseItem*	_Item;
		tgSInt32	_SlotID;
	};
	struct SItemInsideSlot
	{
		tgBool		_ItemInInventory = false;
		tgSInt32	_SlotID = -1;
	};
	// Constructor / Destructor

	CInventory( CPlayer* owner );
	~CInventory( void );

	//////////////////////////////////////////////////////////////////////////

	void								Update( const tgFloat DeltaTime );

	//////////////////////////////////////////////////////////////////////////

	void								DrawImGUI( void );

	void								SwitchItemSlots( tgSInt32 CurrentSlotID );

	void								DropItem(tgSInt32 SlotID);
	void								CreateInventory();
	void								ClearInventoryItems();
	void								ClearInventory();

	void								PickupItem();

	void								InputEvent(const tgInput::EType Type, const tgInput::SEvent* pEvent) override;

	void								Render2D();

	void								ToggleInventory();
	void								EquipItem();
	void								UnequipItem();

	tgBool								IsItemInInventory(tgSInt32 ItemID, SItemInsideSlot& ReturnVal);

private:

	void								DropPayload(SDragDropPayload Payload, tgSInt32 RecieverID);

	tgBool								IsInventoryslotNotEmpty(tgSInt32 SlotID);

	void								ResetLatestIDs( void );

	CPlayer*							m_pPlayer;

	tgCCollision*						m_pItemCollision;

	//CInventorySlot*					Items[9];

	tgSInt32							m_RowLenght;
	tgSInt32							m_AmountOfSlots;
	tgSInt32							m_LatestPushedSlotID;
	tgSInt32							m_LatestPushedItemID;
	tgSInt32							m_SelectedSlotID;

	tgUInt16							m_WindowHeight;
	tgUInt16							m_WindowWidth;

	EMenuState							m_MenuState;

	tgCQuad*							m_pInventoryQuad;
	tgCQuad*							m_pMouseQuad;

	std::vector<CInventorySlot*>		m_InventorySlots;

	CInventorySlot*						m_pSelectedInventorySlot;

	tgCLine3D							m_LastLinePrinted;

	SDragDropPayload					m_DragDropPayload;

	tgBool								m_DropAll;
	tgBool								m_InventoryVisible;
	tgBool								m_ItemEquipped;

};	// CInventory
