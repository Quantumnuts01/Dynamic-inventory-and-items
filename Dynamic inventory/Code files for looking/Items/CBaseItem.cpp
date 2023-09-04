
#include	<tgSystem.h>

#include	"CBaseItem.h"
#include	"ItemComponents/CBaseItemComponent.h"


#include	"../../Managers/CModelManager.h"
#include	"../../Managers/CItemManager.h"
CBaseItem::CBaseItem(tgSInt32 ID, SItemData Data):
	m_pItemModel(nullptr)
{
	m_ID	= ID;
	m_Data	= Data;
	m_ItemName = tgCString("%d", this);
}

CBaseItem::~CBaseItem(void)
{
	CModelManager::GetInstance().DestroyModel(m_ItemName);
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
	}
}



void CBaseItem::Update(const tgFloat Deltatime)
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Update(Deltatime);
	}
}

void CBaseItem::SetItemPosition(tgCV3D Position)
{
	m_pItemModel->GetTransform().GetMatrixLocal().Pos = Position;
	m_pItemModel->Update();
}

void CBaseItem::SpawnItemModel(tgCV3D SpawnPos)
{
	if (!m_pItemModel)
	{
		m_pItemModel = CModelManager::GetInstance().LoadModel(m_Data._ModelPath.String(), m_ItemName, true);
		/*dynamic_cast<CBaseItem*>( */m_pItemModel->GetTransform().GetMatrixLocal().Pos = SpawnPos/* ->GetAssetUserData())*/;
		


		m_pItemModel->Update();
	}
}

void CBaseItem::DespawnItemModel()
{
	if (m_pItemModel)
	{
		CModelManager::GetInstance().DestroyModel(m_pItemModel);
		m_pItemModel = nullptr;
	}
}

void CBaseItem::ItemSelected(CPlayer* Owner)
{
}



