
#include	<tgSystem.h>
#include	"CInventory.h"
#include	"../Items/CBaseItem.h"
#include	"../CPlayer.h"
#include	"../../Managers/CItemManager.h"
#include	"../../Managers/CWorldManager.h"

#include	<tgCTextureManager.h>
#include	<tgMath.h>
#include	<tgCString.h>
#include	<tgCMutex.h>
#include	<tgCModel.h>
#include	<tgCCollision.h>
#include	<tgCSphere.h>
#include	<tgCQuadManager.h>
#include	<tgCDebugManager.h>
#include	<tgCCore.h>


CInventory::CInventory( CPlayer* owner )
	: m_InventorySlots({})
	, m_MenuState(STATE_MAIN_MENU)
	, m_RowLenght(10)
	, m_LatestPushedSlotID(-1)
	, m_LatestPushedItemID(0)
	, m_SelectedSlotID(0)
	, m_AmountOfSlots(20)
	, m_pPlayer(nullptr)
	, m_pItemCollision(nullptr)
	, m_pSelectedInventorySlot(nullptr)
	, m_pInventoryQuad(nullptr)
	, m_pMouseQuad(nullptr)
	, m_DropAll(false)
	, m_InventoryVisible(true)
	, m_ItemEquipped(false)
	, m_DragDropPayload({})
{

	m_WindowHeight = tgCCore::GetInstance().GetWindowHeight();
	m_WindowWidth = tgCCore::GetInstance().GetWindowWidth();
	m_pMouseQuad = tgCQuadManager::GetInstance().Create("Mouse", tgCV3D::Zero, tgCV2D(m_WindowHeight / 10, m_WindowHeight / 10), tgCColor::White);
	m_pMouseQuad->GetMaterial().GetColor().a = 0;

	m_pPlayer = owner;
	CreateInventory();
	////This whole thing is a clusterfrick of bad math but it kind of works
	//tgFloat X = -1 * (((tgFloat)m_RowLenght - 1) / 2);
	//tgSInt32 Y = 1;
	//
	////These two sets the whole inventory fields dimensions
	//tgFloat QuadWidth = WindowWidth / /*20*/19;
	//tgFloat QuadHeight = WindowHeight / /*19*/18;
	//
	////This is the margin in between slots
	//tgFloat SlotMargin = 5;
	//
	////This sets the origin of the quads
	//tgFloat Yoffset = (WindowHeight / 2) - ((tgFloat)WindowHeight / 8) + ((QuadHeight + SlotMargin)/3);
	//tgFloat d =((tgFloat)m_AmountOfSlots / (tgFloat)m_RowLenght);
	//tgSInt32 Rows = tgMathCeil(d);
	//tgCV2D QuadDimensions = tgCV2D(QuadWidth, QuadHeight);
	//
	////This thing Places all of the tiny rectangles
	//for (tgSInt32 i = 0; i < m_AmountOfSlots; i++)
	//{
	//	tgSInt32 mod = ((i + 1) % m_RowLenght);
	//	
	//	tgCV3D QuadPos = tgCV3D((WindowWidth / 2) + (X * (QuadWidth + SlotMargin)), (WindowHeight / 2) + Yoffset - ((Rows - Y) * (QuadHeight + SlotMargin)), 0);
	//	
	//	m_InventorySlots.push_back(new CInventorySlot(i));
	//	m_InventorySlots.back()->SetSlotQuad( tgCQuadManager::GetInstance().Create(tgCString("Slot%i", i) , QuadPos, QuadDimensions, tgCColor::White),
	//		QuadPos, QuadDimensions);
	//	X++;
	//	if (mod == 0 && i < m_AmountOfSlots)
	//	{
	//		X = -1 * (((tgFloat)m_RowLenght - 1) / 2);
	//		Y += 1;
	//	}
	//}
	//tgFloat HalfRows = (tgFloat)Rows / 2;
	//tgFloat Yoriginoffset = Yoffset - (((QuadHeight + SlotMargin) / 2) * (Rows - 1));
	//tgFloat yoffset = Yoriginoffset - (HalfRows * ((QuadHeight + SlotMargin) / 2));
	//
	////InventoryQuadPos is what moves the quad in the back to the correct place
	//tgCV3D InventoryQuadPos = tgCV3D((WindowWidth / 2), (WindowHeight / 2) + Yoriginoffset, 0);
	//tgCV2D InventoryQuadDimensions = tgCV2D(((QuadWidth + SlotMargin) * m_RowLenght), Rows * (QuadHeight + SlotMargin) );
	//m_pInventoryQuad = tgCQuadManager::GetInstance().Create("Inventory", InventoryQuadPos, InventoryQuadDimensions, tgCColor::Gray);
	//
	//m_pSelectedInventorySlot = m_InventorySlots[0];
	//m_pSelectedInventorySlot->GetQuad()->GetMaterial().SetColor(tgCColor::Silver);

	m_pItemCollision = new tgCCollision(true);

	tgInput::AddListener(this);

}
void CInventory::CreateInventory()
{
	//This whole thing is a clusterfrick of bad math but it kind of works
	tgFloat X = -1 * (((tgFloat)m_RowLenght - 1) / 2);
	tgSInt32 Y = 1;

	//These two sets the whole inventory fields dimensions
	tgFloat QuadWidth = m_WindowWidth / /*20*/19;
	tgFloat QuadHeight = m_WindowHeight / /*19*/18;

	//This is the margin in between slots
	tgFloat SlotMargin = 5;

	//This sets the origin of the quads
	tgFloat Yoffset = (m_WindowHeight / 2) - ((tgFloat)m_WindowHeight / 8) + ((QuadHeight + SlotMargin) / 3);
	tgFloat d = ((tgFloat)m_AmountOfSlots / (tgFloat)m_RowLenght);
	tgSInt32 Rows = tgMathCeil(d);
	tgCV2D QuadDimensions = tgCV2D(QuadWidth, QuadHeight);

	//This thing Places all of the tiny rectangles
	for (tgSInt32 i = 0; i < m_AmountOfSlots; i++)
	{
		tgSInt32 mod = ((i + 1) % m_RowLenght);

		tgCV3D QuadPos = tgCV3D((m_WindowWidth / 2) + (X * (QuadWidth + SlotMargin)), (m_WindowHeight / 2) + Yoffset - ((Rows - Y) * (QuadHeight + SlotMargin)), 0);

		m_InventorySlots.push_back(new CInventorySlot(i));
		m_InventorySlots.back()->SetSlotQuad(tgCQuadManager::GetInstance().Create(tgCString("Slot%i", i), QuadPos, QuadDimensions, tgCColor::White),
			QuadPos, QuadDimensions);
		X++;
		if (mod == 0 && i < m_AmountOfSlots)
		{
			X = -1 * (((tgFloat)m_RowLenght - 1) / 2);
			Y += 1;
		}
	}
	tgFloat HalfRows = (tgFloat)Rows / 2;
	tgFloat Yoriginoffset = Yoffset - (((QuadHeight + SlotMargin) / 2) * (Rows - 1));
	tgFloat yoffset = Yoriginoffset - (HalfRows * ((QuadHeight + SlotMargin) / 2));

	//InventoryQuadPos is what moves the quad in the back to the correct place
	tgCV3D InventoryQuadPos = tgCV3D((m_WindowWidth / 2), (m_WindowHeight / 2) + Yoriginoffset, 0);
	tgCV2D InventoryQuadDimensions = tgCV2D(((QuadWidth + SlotMargin) * m_RowLenght), Rows * (QuadHeight + SlotMargin));
	m_pInventoryQuad = tgCQuadManager::GetInstance().Create("Inventory", InventoryQuadPos, InventoryQuadDimensions, tgCColor::Gray);
	m_SelectedSlotID = 0;
	m_pSelectedInventorySlot = m_InventorySlots[0];
	m_pSelectedInventorySlot->GetQuad()->GetMaterial().SetColor(tgCColor::Silver);
}

void CInventory::ClearInventory()
{
	tgCQuadManager::GetInstance().Destroy(&m_pInventoryQuad);
	std::vector<CInventorySlot*> Temp = m_InventorySlots;
	m_InventorySlots = std::vector<CInventorySlot*>({});
	for (size_t i = 0; i < Temp.size(); i++)
	{
		delete Temp[i];
	}
}

CInventory::~CInventory(void)
{
	tgInput::RemoveListener(this);

	delete m_pItemCollision;
	tgCQuadManager::GetInstance().Destroy(&m_pInventoryQuad);
	std::vector<CInventorySlot*> Temp = m_InventorySlots;
	m_InventorySlots = std::vector<CInventorySlot*>({});
	for (size_t i = 0; i < Temp.size(); i++)
	{
		delete Temp[i];
	}
	tgCQuadManager::GetInstance().Destroy(&m_pMouseQuad);


}

void CInventory::Update(const tgFloat DeltaTime)
{
	for (size_t i = 0; i < m_InventorySlots.size(); i++)
	{
		m_InventorySlots[i]->Update(DeltaTime);
	}
	tgCDebugManager::GetInstance().AddLine3D(m_LastLinePrinted,tgCColor::Red);
}

void CInventory::DrawImGUI( void )
{
	if (this)
	{
		if (ImGui::Button(tgCString("Add Item 1 to Slot: %i", m_SelectedSlotID).String(), {150,50}))
		{
			m_InventorySlots[m_SelectedSlotID]->AddItem(1, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(tgCString("Add Item 2 to Slot: %i", m_SelectedSlotID).String(), { 150,50 }))
		{
			m_InventorySlots[m_SelectedSlotID]->AddItem(2, 1);
		}
		if (ImGui::Button(tgCString("Add Item 3 to Slot: %i", m_SelectedSlotID).String(), { 150,50 }))
		{
			m_InventorySlots[m_SelectedSlotID]->AddItem(3, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(tgCString("clear inventory %i", m_SelectedSlotID).String(), { 150,50 }))
		{
			ClearInventory();
		}
		if (ImGui::Button(tgCString("Add Itemslot", m_SelectedSlotID).String(), { 150,50 }))
		{
			m_AmountOfSlots++;
			ClearInventory();
			CreateInventory();
		}
		ImGui::SameLine();
		if (ImGui::Button(tgCString("Remove Itemslot", m_SelectedSlotID).String(), { 150,50 }))
		{
			m_AmountOfSlots--;
			ClearInventory();
			CreateInventory();
		}
		if (ImGui::Button(tgCString("Add itemrowlenght", m_SelectedSlotID).String(), { 150,50 }))
		{
			m_RowLenght++;
			ClearInventory();
			CreateInventory();
		}
		ImGui::SameLine();
		if (ImGui::Button(tgCString("Remove itemrowlenght", m_SelectedSlotID).String(), { 150,50 }))
		{
			m_RowLenght--;
			ClearInventory();
			CreateInventory();
		}
		tgCString ID = tgCString("%i", m_SelectedSlotID);
		ImGui::Text(tgCString("Selected Itemslot ID: %s",ID ).String());
		if (m_SelectedSlotID >= 0 && m_SelectedSlotID < m_InventorySlots.size())
		{
			{
				if (m_InventorySlots.at(m_SelectedSlotID)->GetItemStack())
				{
					ImGui::BulletText(tgCString("Item Name: %s", m_InventorySlots[m_SelectedSlotID]->GetItemStack()->GetItemName().String()).String());
					ImGui::BulletText("The item name is the pointer to the object\nThis ensures that every object has a uniqe ID");
				}
			}
			ImGui::BeginChildFrame(ImGuiID("DD"), { 300,300 });
			ImGui::Text("Controls:");
			ImGui::Text("");
			ImGui::BulletText("Toggle mouse: 'I'\n");
			ImGui::Text("");
			ImGui::BulletText("Add item nr 1 to an itemslot\nby hovering the mouse over itand\npressing Left-Mousebutton\nFor the other items select a slot and\npress the imgui button");
			ImGui::Text("");
			ImGui::BulletText("Equip Item 3 with the 'f' key\n");
			ImGui::Text("");
			ImGui::BulletText("Drag and drop items with\nthe Right-Mousebutton\n");
			ImGui::Text("");
			ImGui::BulletText("Select previous slot in inventory: '1'\nOr scroll up\n");
			ImGui::Text("");
			ImGui::BulletText("Select next slot in inventory: '2'\nOr scroll down\n");
			ImGui::Text("");
			ImGui::BulletText("Drop item: 'Q',\nor drag it outside the inventory\n");
			ImGui::Text("");
			ImGui::BulletText("Pick up item: 'E'\n");
			ImGui::EndChildFrame();
		}
	}
}

void CInventory::SwitchItemSlots(tgSInt32 CurrentSlotID)
{
	if (CurrentSlotID == m_LatestPushedSlotID && m_LatestPushedSlotID <! 0)
	{
		ResetLatestIDs();
		return;
	}
	ResetLatestIDs();
}

void CInventory::DropItem(tgSInt32 SlotID)
{
	if (!m_InventorySlots[SlotID]->GetItemStack())
	{
		return;
	}
	if (m_ItemEquipped)
	{
		UnequipItem();
	}
	tgCWorld& World = *CWorldManager::GetInstance().GetWorld("Collision");

	tgCV3D ppos = m_pPlayer->GetPlayerPosition();
	ppos.y += 1;

	tgCLine3D Line = tgCLine3D(ppos, ppos + (m_pPlayer->GetPlayerForward()*5));
	m_pItemCollision->Clear();
	m_pItemCollision->SetType(tgCMesh::EType::TYPE_WORLD);
	if (m_pItemCollision->LineAllMeshesInWorld(Line, World))
	{
		tgCV3D Position = m_pItemCollision->GetLocalIntersection();
		Position.y += 0.1;
		if (m_InventorySlots[SlotID]->GetItemStack()->GetItemData()._StackSize > 1 && !m_DropAll)
		{
			CItemManager::GetInstance().CreateItemInWorld(m_InventorySlots[SlotID]->GetItemStack()->GetItemID(), Position);
			m_InventorySlots[SlotID]->GetItemStack()->GetItemData()._StackSize -= 1;
		}
		else
		{
			CItemManager::GetInstance().AddItemToWorld(m_InventorySlots[SlotID]->GetItemStack(), Position);
			m_InventorySlots[SlotID]->GetItemStack()->SpawnItemModel(Position);
			m_InventorySlots[SlotID]->SetItemStack(nullptr);
			m_InventorySlots[SlotID]->UpdateItemDesc();
		}
	}
	m_LastLinePrinted = Line;
}

void CInventory::PickupItem()
{
	//Tengine has its own collision so i will try to explain what happens here

	//First you will need a reference to the Collision world/mesh
	tgCWorld& World = *CWorldManager::GetInstance().GetWorld("Collision");

	tgCV3D PlayerPos = m_pPlayer->GetPlayerPosition();
	PlayerPos.y += 1;

	//Then you make a line from the player pointing towards where you look
	tgCLine3D Line = tgCLine3D(PlayerPos, PlayerPos + (m_pPlayer->GetPlayerForward() * 5));

	//in LineItemsInWorld is where the rest happens

	CBaseItem* HitItem = CItemManager::GetInstance().LineItemsInWorld(Line, m_pItemCollision);

	//This line is only here to show where the linetrace goes
	m_LastLinePrinted = Line;
	if (HitItem)
	{
		SItemInsideSlot RetItemIsInside;
		if (IsItemInInventory(HitItem->GetItemID(), RetItemIsInside))
		{
			EReturnItemStatus ItemsLeft = m_InventorySlots.at(RetItemIsInside._SlotID)->AddItemExisting(HitItem);
			if (ItemsLeft == EReturnItemStatus::NO_ITEMS_LEFT || ItemsLeft == EReturnItemStatus::STACK_MOVED)
			{
				CItemManager::GetInstance().DestroyItemInWorld(HitItem->GetItemName());
				return;
			}
			
		}
		
		for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
		{
			if (m_InventorySlots[i]->GetItemStack() && HitItem)
			{
				if (m_InventorySlots[i]->GetItemStack()->GetItemID() == HitItem->GetItemID())
				{
					tgSInt32 ItemsLeft = m_InventorySlots[i]->AddItemExisting(HitItem);
					if (ItemsLeft < 0)
					{
						CItemManager::GetInstance().DestroyItemInWorld(HitItem->GetItemName());
						break;
					}
					else if (ItemsLeft > 0)
					{
						HitItem->SetItemStackSize(ItemsLeft);
					}
				}
			}
			else if (HitItem)
			{
				HitItem->DespawnItemModel();
				m_InventorySlots[i]->SetItemStack(HitItem);
				m_InventorySlots[i]->UpdateItemDesc();
				CItemManager::GetInstance().GetItemMap().erase(HitItem->GetItemName());
				break;
			}
		}
	}
}

void CInventory::DropPayload(SDragDropPayload Payload, tgSInt32 RecieverID)
{
	//When the m_DragDropPayload is dropped the first thing is to see if it is dropped on its own quad if not then the next step happens
	if (Payload._SlotID == RecieverID)
	{
		return;
	}
	//in AddItemExisting the payloads itemstack is updated aswell as the possibly new slot
	EReturnItemStatus ItemsLeft = m_InventorySlots.at(RecieverID)->AddItemExisting(Payload._Item);

	//first we see if the stack can fit on the new itemstack/ in the new slot

	if ( ItemsLeft == EReturnItemStatus::NO_ITEMS_LEFT )
	{
		//If the stack was absorbed into another stack the old one is deleted and set to a nullptr and update the visual elements on the UI
		delete Payload._Item;
		m_InventorySlots.at(Payload._SlotID)->SetItemStack(nullptr);
		m_InventorySlots.at(Payload._SlotID)->UpdateItemDesc();
		m_InventorySlots.at(RecieverID)->UpdateItemDesc();
		return;
	}
	else if ( ItemsLeft == EReturnItemStatus::STACK_MOVED )
	{
		//If the stack was moved to a new slot we set the payload to a nullptr and update the visual elements on the UI
		m_InventorySlots.at(Payload._SlotID)->SetItemStack(nullptr);
		m_InventorySlots.at(Payload._SlotID)->UpdateItemDesc();
		m_InventorySlots.at(RecieverID)->UpdateItemDesc();
		return;
	}
	else if (ItemsLeft == EReturnItemStatus::ITEMS_LEFT )
	{
		//If there are items left in the Payload stack then the we just update the UI for the two itemslots
		m_InventorySlots.at(Payload._SlotID)->UpdateItemDesc();
		m_InventorySlots.at(RecieverID)->UpdateItemDesc();
	}

	
}

tgBool CInventory::IsInventoryslotNotEmpty(tgSInt32 SlotID)
{
	if (m_InventorySlots.size() > 0 && SlotID >= 0)
		if (m_InventorySlots[SlotID]->GetItemStack())
			return true;

	return false;
}

void CInventory::ToggleInventory()
{
	m_pInventoryQuad->GetMaterial().GetColor().a = !m_InventoryVisible;
	for (size_t i = 0; i < m_InventorySlots.size(); i++)
	{
		m_InventorySlots[i]->GetQuad()->GetMaterial().GetColor().a = !m_InventoryVisible;
	}
	m_InventoryVisible = !m_InventoryVisible;
}

void CInventory::EquipItem()
{
	if (m_InventorySlots[m_SelectedSlotID]->GetItemStack())
	{
		if (m_InventorySlots[m_SelectedSlotID]->GetItemStack()->GetIsEquipable())
		{
			m_ItemEquipped = true;
			m_InventorySlots[m_SelectedSlotID]->GetItemStack()->SpawnItemModel(tgCV3D::PositiveY);
			m_InventorySlots[m_SelectedSlotID]->GetItemStack()->ItemSelected(m_pPlayer);
		}
	}
}

void CInventory::UnequipItem()
{
	if (m_InventorySlots[m_SelectedSlotID]->GetItemStack() && m_ItemEquipped)
	{
		m_ItemEquipped = false;
		m_InventorySlots[m_SelectedSlotID]->GetItemStack()->DespawnItemModel();
	}
}

tgBool CInventory::IsItemInInventory(tgSInt32 ItemID, SItemInsideSlot& ReturnVal)
{
	//Here whe look if another itemstack of the same itemtype exists
	for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
	{
		if (m_InventorySlots[i]->GetItemStack() && m_InventorySlots[i]->GetItemStack()->GetItemID() == ItemID)
		{
			ReturnVal._ItemInInventory = true;
			ReturnVal._SlotID = i;
			return true;
		}
	}
	return false;
}

void CInventory::ResetLatestIDs( void )
{
	m_LatestPushedSlotID = -1;
	m_LatestPushedItemID = 0;
}

void CInventory::InputEvent(const tgInput::EType Type, const tgInput::SEvent* pEvent)
{
	//This is for the inventory keybindings, the input event is tengine´s way of regestering keypresses
	switch (Type)
		{
		case tgInput::TYPE_KEY_DOWN:
			switch (pEvent->Keyboard.VKey)
			{
			case 'Q':
				DropItem(m_pSelectedInventorySlot->GetSlotID());
				break;

			case 'E':
			{
				PickupItem();
			}
			break;
			case 'F':
			{
				if (!m_ItemEquipped)
					EquipItem();
				else
					UnequipItem();
			}
			break;
			case VK_SHIFT:
				m_DropAll = true;
				break;
			default:
				break;

			}
			break;
		case tgInput::TYPE_KEY_UP:
			if (pEvent->Keyboard.VKey == VK_SHIFT)
			{
				m_DropAll = false;
			}

			break;

		case tgInput::TYPE_MOUSE_DOWN:

			if (pEvent->Mouse.ButtonId == 1 && m_InventoryVisible)
			{
				//This looks for if the cursor hovers over one of the squares when LMouse-button is pressed
				for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
				{
					if (m_InventorySlots[i]->GetQuad()->PointInside(pEvent->CurrPos))
					{
						//if it hovers over one it tries to add one item
						m_InventorySlots[i]->AddItem(1, 1);
						break;
					}
				}
				//this part is if the gun is equipped
				//if it is equipped it shoots
				if (m_InventorySlots[m_SelectedSlotID]->GetItemStack() && m_ItemEquipped)
				{
					m_InventorySlots[m_SelectedSlotID]->GetItemStack()->UseItem();
				}
			}
			if (pEvent->Mouse.ButtonId == 2 && m_InventoryVisible)
				//Here we check if the mouse is hovering over a square and if so 
				//if that slot has an item that can be moved from slot to slot or dropped
				//if that is the case that item is set as m_DragDropPayload
				for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
				{
					if (m_InventorySlots[i]->GetQuad()->PointInside(pEvent->CurrPos ) && m_InventorySlots[i]->GetItemStack())
					{
						m_pMouseQuad->GetMaterial().GetColor().a = 0.5;
						m_pMouseQuad->GetMaterial().SetColormap(*m_InventorySlots[i]->GetQuad()->GetMaterial().GetColormap());
						m_DragDropPayload._Item = m_InventorySlots[i]->GetItemStack();
						m_DragDropPayload._SlotID = i;
						return;
					}
				}
			break;
		case tgInput::TYPE_MOUSE_UP:
			if (pEvent->Mouse.ButtonId == 2 && m_InventoryVisible && m_DragDropPayload._Item)
			{
				//This checks when RMouse-button is released what it should do with the m_DragDropPayload
				//the item is dropped´if the cursor is hovering outside the inventory quad otherwise it checks
				//if the item can be stacked or should remain in its current slot
				tgBool Dropped = false;
				for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
				{
					if (m_InventorySlots[i]->GetQuad()->PointInside(pEvent->CurrPos))
					{
						Dropped = true;
						m_pMouseQuad->GetMaterial().GetColor().a = 0.;
						DropPayload(m_DragDropPayload, i);
						break;
					}
				}
				if (m_pInventoryQuad->PointInside(pEvent->CurrPos))
				{
					m_pMouseQuad->GetMaterial().GetColor().a = 0.;
					DropPayload(m_DragDropPayload, m_DragDropPayload._SlotID);
					m_DragDropPayload._Item = nullptr;
				}
				else if (!Dropped)
				{
					m_pMouseQuad->GetMaterial().GetColor().a = 0.;
					DropItem(m_DragDropPayload._SlotID);
					m_DragDropPayload._Item = nullptr;
				}
			}
			break;

		case tgInput::TYPE_MOUSE_MOVE_ABSOLUTE:
			//this code just moves the mouse quad which interacts with the inventory
			if (m_InventoryVisible)
			{
				m_pMouseQuad->SetPos({ pEvent->CurrPos.x,pEvent->CurrPos.y, 1 });
				for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
				{
					if (m_InventorySlots[i]->GetQuad()->PointInside(pEvent->CurrPos))
					{
						m_InventorySlots[i]->Hovered(true);
						continue;
					}
					m_InventorySlots[i]->Hovered(false);
				}
			}
			break;
		case tgInput::TYPE_MOUSE_WHEEL:
		{

			//this allows the scrollwheel to scroll through the inventory
			tgSInt32 NewSlotID;
			if (pEvent->Mouse.Wheel == 120)
			{
				NewSlotID = tgMathClamp(0, m_SelectedSlotID + 1, (tgSInt32)m_InventorySlots.size() - 1);
				if (NewSlotID == m_SelectedSlotID)
					return;
				UnequipItem();
				m_pSelectedInventorySlot->GetQuad()->GetMaterial().SetColor(tgCColor::White);
				m_SelectedSlotID = NewSlotID;
				m_pSelectedInventorySlot = m_InventorySlots[m_SelectedSlotID];
				m_pSelectedInventorySlot->GetQuad()->GetMaterial().SetColor(tgCColor::Silver);
			}
			if (pEvent->Mouse.Wheel == -120)
			{
				NewSlotID = tgMathClamp(0, m_SelectedSlotID - 1, (tgSInt32)m_InventorySlots.size() - 1);
				if (NewSlotID == m_SelectedSlotID)
					return;
				UnequipItem();
				m_pSelectedInventorySlot->GetQuad()->GetMaterial().SetColor(tgCColor::White);
				m_SelectedSlotID = NewSlotID;
				m_pSelectedInventorySlot = m_InventorySlots[m_SelectedSlotID];
				m_pSelectedInventorySlot->GetQuad()->GetMaterial().SetColor(tgCColor::Silver);
			}

		}
		break;
		//case tgInput::TYPE_MOUSE_SINGLE_CLICK:
		//	if (m_InventoryVisible)
		//	{
		//		m_pMouseQuad->SetPos({ pEvent->CurrPos.x,pEvent->CurrPos.y, 1 });
		//		for (tgSInt32 i = 0; i < m_InventorySlots.size(); i++)
		//		{
		//			if (m_InventorySlots[i]->GetQuad()->PointInside(pEvent->CurrPos))
		//			{
		//				m_InventorySlots[i]->Hovered(true);
		//				continue;
		//			}
		//			m_InventorySlots[i]->Hovered(false);
		//		}
		//	}
		//	break;
		}
}

void CInventory::Render2D()
{
	//The Render2D function allows for the inventory to render on top of everything to work as a UI
	if (m_pInventoryQuad)
	{
		m_pInventoryQuad->Render();
	}
	for (size_t i = 0; i < m_InventorySlots.size(); i++)
	{
		if (m_InventorySlots[i])
		{
			m_InventorySlots[i]->Render2D();
		}
		tgCV3D QuadPos = m_InventorySlots[i]->GetQuad()->GetTransform().GetMatrixLocal().Pos;
	}
	if (m_pMouseQuad)
	{
		m_pMouseQuad->Render();
	}
}