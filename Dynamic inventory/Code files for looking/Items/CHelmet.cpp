
#include <tgSystem.h>
#include "CHelmet.h"
#include "ItemComponents/CThrowableItemComponent.h"
#include "Specialization/CPlayer.h"
#include <tgCModel.h>

CHelmet::CHelmet(tgSInt32 ItemID, SItemData ItemData) :CBaseItem(ItemID,ItemData)
{
	m_pComponents.push_back(new CThrowableItemComponent(this));
}
CHelmet::~CHelmet()
{
}

tgBool CHelmet::UseItem(CPlayer* Owner)
{
	m_pComponents[0]->Use();
	return true;
}

void CHelmet::ItemSelected(CPlayer* Owner)
{
	tgCMatrix& rOwnerModelMatrix = Owner->GetPlayerModel()->GetTransform().GetMatrixLocal();
	rOwnerModelMatrix.Scale(1.1, tgCMatrix::COMBINE_PRE_MULTIPLY);
	m_pItemModel->GetTransform().GetMatrixLocal().Pos = rOwnerModelMatrix.Pos;
	Owner->GetPlayerModel()->GetTransform().GetParent()->AddChild(m_pItemModel->GetTransform());
	
}