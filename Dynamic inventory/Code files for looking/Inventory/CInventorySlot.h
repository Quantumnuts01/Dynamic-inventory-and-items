#pragma once


#include	<tgMemoryDisable.h>
#include	<imgui.h>
#include	<string>
#include	<tgMemoryEnable.h>
#include	<tgCString.h>
#include	<tgCV2D.h>
class CItemLibrary;
class CBaseItem;

enum EReturnItemStatus
{
	NO_ITEMS_LEFT,
	ITEMS_LEFT,
	STACK_MOVED,
	NOTHING,
};
class CInventorySlot
{
public:

	// Constructor / Destructor
	CInventorySlot( tgSInt32 ID );
	~CInventorySlot( void );

	//////////////////////////////////////////////////////////////////////////
	
	void					Update(const tgFloat DeltaTime);
	tgBool					AddItem(tgSInt32 ItemID, tgSInt32 ItemAmount);
	tgBool					DrawImGuiButton( void );
	void					ClearItem();

	CBaseItem*				GetItemStack(void);
	tgCQuad*				GetQuad(void) { return m_pSlotQuad; }
	tgSInt32				GetSlotID(void) { return m_SlotID; }

	void					SetItemStack(CBaseItem* NewItemStack) { m_pCurrentItemStack = NewItemStack; }
	void					SetSlotQuad(tgCQuad* pSlotQuad, tgCV3D QuadPos, tgCV2D QuadDimensions);

	void					UpdateItemDesc();
	EReturnItemStatus		AddItemExisting(CBaseItem* Item);

	void					Render2D();

	void					Hovered(tgBool IsHovered);

	//////////////////////////////////////////////////////////////////////////


private:
	tgCTexture*		m_pSlotTex;
	tgCQuad*		m_pSlotQuad;
	tgCTexture*		m_pNumberTex;
	tgCQuad*		m_pNumberQuad;
	CItemLibrary*	m_pLib;
	CBaseItem*		m_pCurrentItemStack;
	tgCTexture*		m_pTex;
	tgBool			m_IsEmpty;
	tgBool			m_Selected;
	tgSInt32		m_SlotID;
	tgSInt32		m_ItemID;
	tgCString		m_ItemDesc;
	tgCV2D			m_Textpos;
	
};	// CInventorySlot
