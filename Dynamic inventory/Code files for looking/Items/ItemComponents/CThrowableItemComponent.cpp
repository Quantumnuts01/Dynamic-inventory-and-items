
#include	<tgSystem.h>
#include	"CThrowableItemComponent.h"
#include	"../CBaseItem.h"
#include	"Managers/CWorldManager.h"
#include	"CApplication.h"
#include	"Camera/CCamera.h"
#include	<tgCCollision.h>
#include	<tgCModel.h>
#include	<tgCMesh.h>
#include	<tgCLine3D.h>

CThrowableItemComponent::CThrowableItemComponent(CBaseItem* pAttachedItem)
	: m_pAttachedItem(nullptr)
	, m_pItemcollision(nullptr)
	, m_Dir(tgCV3D::Zero)
	, m_ShouldUpdate(false)
	, m_X(0)
{
	m_Sphere = tgCSphere(0, 0.1);
	m_pAttachedItem = pAttachedItem;
	m_pItemcollision = new tgCCollision(true);
	m_pWorld = CWorldManager::GetInstance().GetActiveWorld();
}

CThrowableItemComponent::~CThrowableItemComponent()
{
	m_pAttachedItem = nullptr;
}

void CThrowableItemComponent::Use()
{
	m_Dir = CApplication::GetInstance().Get3DCamera()->GetCamera()->GetTransform().GetMatrixLocal().At;
	m_Sphere.SetPos(m_pAttachedItem->GetItemModel()->GetTransform().GetMatrixLocal().Pos);

	m_ShouldUpdate = true;
}

bool CThrowableItemComponent::Collision()
{
	m_pItemcollision->Clear();
	m_pItemcollision->SetType(tgCMesh::EType::TYPE_WORLD);
	/*if (m_pItemcollision->LineAllMeshesInWorld(tgCLine3D(m_Sphere.GetPos(), m_Sphere.GetPos() + m_Dir * 3), *m_pWorld))
	{
		m_pAttachedItem->GetItemModel()->GetTransform().GetMatrixLocal().Pos = m_pItemcollision->GetLocalIntersection();
		m_ShouldUpdate = false;
		return false;
	}*/
	if (m_pItemcollision->SphereAllMeshesInWorld(m_Sphere, *m_pWorld))
	{
		m_pAttachedItem->GetItemModel()->GetTransform().GetMatrixLocal().Pos = m_pItemcollision->GetLocalIntersection();
		m_ShouldUpdate = false;
		return false;
	}
	return true;
}
