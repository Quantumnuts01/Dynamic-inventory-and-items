
#include <tgSystem.h>
#include "CGun.h"
#include "Specialization/CPlayer.h"
#include "Managers/CBulletManager.h"
#include <tgCMatrix.h>
#include <CApplication.h>
#include <Camera/CCamera.h>
#include <tgCModel.h>

CGun::CGun(tgSInt32 ItemID, SItemData ItemData) : CBaseItem(ItemID, ItemData)
{
}

CGun::~CGun()
{
}


void CGun::Update(const tgFloat Deltatime)
{
}

void CGun::Shoot()
{
	CBulletManager::GetInstance().NewBullet(new  SBullet{ m_pItemModel->GetTransform().GetMatrixWorld().At, 5, 3,tgCSphere(m_pItemModel->GetTransform().GetMatrixWorld().Pos, 0.3) });
}
tgBool CGun::UseItem(CPlayer* Owner)
{
	Shoot();
	return true;
}

void CGun::ItemSelected(CPlayer* Owner)
{
	tgCMatrix& rOwnerModelMatrix = Owner->GetPlayerModel()->GetTransform().GetMatrixLocal();
	m_pItemModel->GetTransform().GetMatrixLocal().Pos = rOwnerModelMatrix.Pos;
	Owner->GetPlayerModel()->GetTransform().GetParent()->AddChild(m_pItemModel->GetTransform());
}
