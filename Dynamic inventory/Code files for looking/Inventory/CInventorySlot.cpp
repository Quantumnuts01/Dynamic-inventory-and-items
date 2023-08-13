
#include	<tgSystem.h>

#include	"CInventorySlot.h"

#include	"Managers/CWorldManager.h"
#include	"../Items/CItemLibrary.h"
#include	"../Items/CBaseItem.h"

#include	<tgCTexture.h>
#include	<tgMath.h>
#include	<tgCQuadManager.h>
#include	<tgCTextureManager.h>
#include	<tgCDebugManager.h>


////////////////////////////// CLevel //////////////////////////////
//                                                                //
//  Info:
//                                                                //
//*/////////////////////////////////////////////////////////////////
CInventorySlot::CInventorySlot(tgSInt32 ID)
	: m_pCurrentItemStack(nullptr)
	, m_pSlotQuad(nullptr)
{
	m_IsEmpty = true;
	m_SlotID = ID;
	UpdateItemDesc();

	m_pLib = CItemLibrary::GetInstancePtr();

}	// */ // CInventorySlot


////////////////////////////// ~CInventorySlot //////////////////////////////
//                                                                 //
//  Info:
//                                                                 //
//*//////////////////////////////////////////////////////////////////
CInventorySlot::~CInventorySlot(void)
{
	CBaseItem* pTemp = m_pCurrentItemStack;
	m_pCurrentItemStack = nullptr;
	if (pTemp)
		delete pTemp;
	tgCQuadManager::GetInstance().Destroy(&m_pSlotQuad);


}	// */ // ~CLevel


////////////////////////////// Update //////////////////////////////
//                                                                //
//  Info:
//                                                                //
//*/////////////////////////////////////////////////////////////////
void
CInventorySlot::Update(const tgFloat DeltaTime)
{
	if (m_pCurrentItemStack)
	{
		m_pCurrentItemStack->Update(DeltaTime);
		tgCDebugManager::GetInstance().AddText2D(m_Textpos,tgCColor::White,tgCString("%i", m_pCurrentItemStack->GetItemData()._StackSize));
	}
}	// */ // Update

tgBool CInventorySlot::AddItem(tgSInt32 ItemID, tgSInt32 ItemAmount)
{
	if (m_pCurrentItemStack && m_pCurrentItemStack->GetItemID() == ItemID)
	{
		tgSInt32 ItemStackSize = m_pCurrentItemStack->GetItemData()._StackSize + ItemAmount;
		m_pCurrentItemStack->GetItemData()._StackSize = tgMathClamp<tgSInt32>(0, ItemStackSize, m_pCurrentItemStack->GetItemData()._MaxStackSize);
		UpdateItemDesc();
		return true;
	}
	else if(!m_pCurrentItemStack)
	{
		m_pCurrentItemStack = m_pLib->CreateItem(ItemID);
		m_pCurrentItemStack->GetItemData()._StackSize = ItemAmount;
		UpdateItemDesc();
		return true;
	}
	else
	{
		return false;
	}
	tgSInt32 ItemStackSize = m_pCurrentItemStack->GetItemData()._StackSize;
	m_pCurrentItemStack->GetItemData()._StackSize = tgMathClamp<tgSInt32>(0, ItemStackSize, m_pCurrentItemStack->GetItemData()._MaxStackSize);
	return false;
	
	
}

tgBool CInventorySlot::DrawImGuiButton( void )
{
	if (this)
	{
		if (m_pCurrentItemStack)
		{
			UpdateItemDesc();
		}
		if (ImGui::Button(m_ItemDesc.String(), {100,70}))
		{
			m_Selected = !m_Selected;
			
			return true;
		}

	}
	
	return false;

}

CBaseItem* CInventorySlot::GetItemStack(void)
{
	return m_pCurrentItemStack;
}

void CInventorySlot::SetSlotQuad(tgCQuad* pSlotQuad, tgCV3D QuadPos, tgCV2D QuadDimensions)
{
	if (m_pSlotQuad == nullptr)
	{
		m_pSlotQuad = pSlotQuad;
		m_pSlotTex = pSlotQuad->GetMaterial().GetColormap();
		tgCV3D SlotPos = m_pSlotQuad->GetTransform().GetMatrixLocal().Pos;
		QuadPos.x += QuadDimensions.x * 0.5;
		QuadPos.y += QuadDimensions.y * 0.5;
		m_Textpos.x = QuadPos.x - 7;
		m_Textpos.y = QuadPos.y - 7;
	}
}

void CInventorySlot::UpdateItemDesc()
{
	if (m_pCurrentItemStack)
	{
		m_pTex = tgCTextureManager::GetInstance().Create(m_pCurrentItemStack->GetItemData()._IconPath);
		m_pSlotQuad->GetMaterial().SetColormap(*m_pTex);
		m_ItemDesc = tgCString("Slot ID: %i\nItem ID: %i\nNr items: %i", m_SlotID, m_pCurrentItemStack->GetItemID(), m_pCurrentItemStack->GetItemData()._StackSize);
		
		return;
	}
	m_ItemDesc = tgCString("Slot ID: %i\nItem ID: None\nNr items: None", m_SlotID);
	if (m_pSlotQuad)
	{
		m_pSlotQuad->GetMaterial().SetColormap(*m_pSlotTex);
	}
	
}

EReturnItemStatus CInventorySlot::AddItemExisting(CBaseItem* Item)
{
	if (m_pCurrentItemStack)
	{
		SItemData ItemData = Item->GetItemData();
		SItemData& ItemStack = m_pCurrentItemStack->GetItemData();
		if (Item->GetItemID() == m_pCurrentItemStack->GetItemID())
		{
			tgSInt32 Val = ItemStack._MaxStackSize - (ItemData._StackSize + ItemStack._StackSize);
			if ((ItemData._StackSize + ItemStack._StackSize) <= ItemStack._MaxStackSize)
			{
				m_pCurrentItemStack->SetItemStackSize(ItemStack._StackSize + ItemData._StackSize);
				return EReturnItemStatus::NO_ITEMS_LEFT;
			}
			else if (ItemStack._StackSize < ItemStack._MaxStackSize)
			{
				//This adds items to the current stack and removes them from the other untill the current one is full or the other one is empty
				while (ItemData._StackSize != 0 && ItemStack._StackSize < ItemStack._MaxStackSize)
				{
					ItemData._StackSize -= 1;
					m_pCurrentItemStack->SetItemStackSize(ItemStack._StackSize + 1);
				}
				if (ItemData._StackSize == 0)
				{
					return EReturnItemStatus::NO_ITEMS_LEFT;
				}
			}
			Item->SetItemStackSize(ItemData._StackSize);
			return EReturnItemStatus::ITEMS_LEFT;
		}
	}
	else if (!m_pCurrentItemStack)
	{
		m_pCurrentItemStack = Item;
		return EReturnItemStatus::STACK_MOVED;
	}

	return EReturnItemStatus::NOTHING;
}

void CInventorySlot::Render2D()
{
	m_pSlotQuad->Render();
}

void CInventorySlot::Hovered(tgBool IsHovered)
{
	if (IsHovered)
	{
		m_pSlotQuad->GetMaterial().GetColor().a = .5;
	}
	else
	{
		m_pSlotQuad->GetMaterial().GetColor().a = 1;
	}
	
}


