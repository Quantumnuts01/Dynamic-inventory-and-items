
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
#include	<tgCDebugManager.h>

CThrowableItemComponent::CThrowableItemComponent(CBaseItem* pAttachedItem)
	: m_pAttachedItem(nullptr)
	, m_pItemcollision(nullptr)
	, m_Dir(tgCV3D::Zero)
	, m_ShouldUpdate(false)
	, m_X(0)
	, m_Speed(1)
{
	m_Sphere = tgCSphere(0, 2);
	m_pAttachedItem = pAttachedItem;
	m_pItemcollision = new tgCCollision(true);
	m_pWorld = CWorldManager::GetInstance().GetActiveWorld();
}

CThrowableItemComponent::~CThrowableItemComponent()
{
	m_pAttachedItem = nullptr;
	delete m_pItemcollision;
}

void CThrowableItemComponent::Use()
{
	m_pAttachedItem->GetItemModel()->Update();
	m_Dir = m_pAttachedItem->GetItemModel()->GetTransform().GetMatrixWorld().At;
	m_Sphere.SetPos(m_pAttachedItem->GetItemModel()->GetTransform().GetMatrixWorld().Pos);

	m_ShouldUpdate = true;
}

void CThrowableItemComponent::Update(tgFloat DeltaTime)
{
	if (m_ShouldUpdate && m_Speed > 0.1)
	{
		//if (Collision())
		//	return; 
		tgCV3D& ItemPos = m_pAttachedItem->GetItemModel()->GetTransform().GetMatrixLocal().Pos;
		ItemPos += (m_Dir * m_Speed * DeltaTime);
		m_pAttachedItem->GetItemModel()->Update();
		m_Dir.y -= DeltaTime * 5;
		//m_Speed -= DeltaTime;
		tgCDebugManager::GetInstance().AddLineSphere(m_Sphere, tgCColor::Red);
	}
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
		return true;
	}
	return false;
}
