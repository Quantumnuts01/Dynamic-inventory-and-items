
#include <tgSystem.h>
#include "CBulletManager.h"
#include <tgCDebugManager.h>
CBulletManager::CBulletManager()
{
}
CBulletManager::~CBulletManager()
{

	for (size_t i = 0; i < m_Bullets.size(); i++)
	{ 
		delete m_Bullets[i];
		m_Bullets.erase(m_Bullets.begin() + i);
		i--;
	}
}
void CBulletManager::NewBullet(SBullet* NewBullet)
{
	m_Bullets.push_back(NewBullet);
}

void CBulletManager::Update(const tgFloat Deltatime)
{
	for (size_t i = 0; i < m_Bullets.size(); i++)
	{
		tgCV3D newpos = m_Bullets[i]->_Sphere.GetPos() + (m_Bullets[i]->_Dir * m_Bullets[i]->_Speed * Deltatime);
		m_Bullets[i]->_Sphere.SetPos(newpos);
		tgCDebugManager::GetInstance().AddLineSphere(m_Bullets[i]->_Sphere, tgCColor::Red);
		m_Bullets[i]->_Lifetime -= Deltatime;
		if (m_Bullets[i]->_Lifetime <= 0)
		{
			delete m_Bullets[i];
			m_Bullets.erase(m_Bullets.begin() + i);
		}
	}
}
