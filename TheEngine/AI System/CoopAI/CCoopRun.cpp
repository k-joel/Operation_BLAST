#ifndef CCoopRun_h__
#define CCoopRun_h__

#include "../CState.h"
#include "../../Camera/Camera.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Game Objects/CObjectManager.h"

void	CCoopRun::Enter()
{
	m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("walk");
}

int		CCoopRun::Update(float fElapsedTime)
{
	m_pNPC->MoveTowardsTarget();

	return 0;
}
float	CCoopRun::GetUtility(void )
{
	return (!m_pNPC->GetInDistance(150.0f)) ? 0.8f : 0.0f;
}
void	CCoopRun::Exit()
{

}
#endif // CCoopRun_h__