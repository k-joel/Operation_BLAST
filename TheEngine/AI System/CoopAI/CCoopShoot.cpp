#ifndef CCoopShoot_h__
#define CCoopShoot_h__

#include "../CState.h"
#include "../../Camera/Camera.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Game Objects/CObjectManager.h"

void	CCoopShoot::Enter()
{
	m_pNPC->SelectTarget(CObjectManager::GetInstance()->GetEnemyList(),false);
}

int	CCoopShoot::Update(float fElapsedTime)
{
	((CPlayer*)(m_pNPC))->SetFlameOn(true);

	if(rand() % 4 == 2 && m_pNPC->GetIsOnGround())
		m_pNPC->SetVelocity(0.0f,0.0f,0.0f);

	if(((CPlayer*)m_pNPC)->GetReadytoFire())
	{
		if(m_pNPC->GetCurrentTarget()->GetPosition().y > m_pNPC->GetPosition().y + 1.05f && m_pNPC->GetCurrentTarget()->GetPosition().x == m_pNPC->GetPosition().x )
		{
			((CPlayer*)m_pNPC)->Shoot(STRAIGHT_UP,NORMAL_FIRE);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("shootUp");
		}
		else if(m_pNPC->GetCurrentTarget()->GetPosition().y > m_pNPC->GetPosition().y + 1.05f )
		{
			((CPlayer*)m_pNPC)->Shoot(ANGLE_UP,NORMAL_FIRE);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalUp");
		}
		else if(m_pNPC->GetCurrentTarget()->GetPosition().y  + 1.05f < m_pNPC->GetPosition().y && m_pNPC->GetCurrentTarget()->GetPosition().x == m_pNPC->GetPosition().x )
		{
			((CPlayer*)m_pNPC)->Shoot(STRAIGHT_DOWN,NORMAL_FIRE);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("shootDown");
		}
		else if(m_pNPC->GetCurrentTarget()->GetPosition().y  + 1.05f < m_pNPC->GetPosition().y  )
		{
			((CPlayer*)m_pNPC)->Shoot(ANGLE_DOWN,NORMAL_FIRE);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalDown");
		}
		else
		{
			((CPlayer*)m_pNPC)->Shoot(STRAIGHT_FORWARD,NORMAL_FIRE);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("shootForward");
		}
	}

	((CPlayer*)(m_pNPC))->SetFlameOn(false);

	return 0;
}
float	CCoopShoot::GetUtility(void )
{
	return (m_pNPC->GetInDistance(150.0f)) ? 0.98f : 0.0f;
}
void	CCoopShoot::Exit()
{

}
#endif // CCoopShoot_h__