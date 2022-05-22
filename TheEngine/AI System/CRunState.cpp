#include "CState.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../../States/CGameState.h"
#include "../Collision/CCollisionTests.h"
#include "../Camera/Camera.h"
#include "../Game Objects/Enemy/CAssaulter.h"
#include "../Game Objects/Enemy/CEnemy.h"

bool bFirstTime = true;
//Enter: DOes Nothing
void CRunState::Enter()
{

	m_bTheWaitingBool = false;
	m_bFailSafe			= false;

}
//GetUtility: Checks various conditions, if it succeeded it returns a value, if not it returns a ZERO
float	CRunState::GetUtility(void )
{
	if(!m_pNPC->GetCurrentTarget())
		return 0.0f;

	if(m_pNPC->GetType() == OBJ_ASSAULTER)
		return ( (!m_pNPC->GetInDistance(150.0f)  ||  !((CAssaulter*)m_pNPC)->CheckAngles()  || CCollisionTests::IntersectFrustrumSphere2(m_pNPC->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()) == CT_OUT ) && !((CEnemy*)m_pNPC)->GetIsArriving() &&  (m_pNPC->GetIsOnGround() || m_pNPC->GetIsOnPlatform())) ? 0.8f : 0.1f;
	return (!m_pNPC->GetInDistance(0.0f) && !((CEnemy*)m_pNPC)->GetIsArriving() &&  (m_pNPC->GetIsOnGround() || m_pNPC->GetIsOnPlatform())) ? 0.8f : 0.0f;

}
//Update: Moves closer to it target
int CRunState::Update(float fElapsedTime)
{
	static float fTimer = 0.0f;
	fTimer += fElapsedTime;

	
	float fRespawnChecker = 0.0f;
	if(((CPlayer*)m_pNPC->GetCurrentTarget())->GetRespawning())
		fRespawnChecker = 5.0f;
	else
		fRespawnChecker = 2.0f;

	if(((CEnemy*)m_pNPC)->GetRunTimer() > fRespawnChecker  )
	{
		((CEnemy*)m_pNPC)->SetRunTimer(0.00f);// = 0.00f;
		if(m_pNPC->GetCurrentTarget() && !m_pNPC->GetCurrentTarget()->GetIsDead() &&
			( CCollisionTests::IntersectFrustrumSphere2(m_pNPC->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()) != CT_IN
			|| (m_pNPC->GetType() == OBJ_STRIKER) ? !m_pNPC->GetInDistance(0.0f) : (!m_pNPC->GetInDistance(150.0f) ||  !((CAssaulter*)m_pNPC)->CheckAngles()) ))
		{
			((CEnemy* )m_pNPC)->SetFirstTime(false);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("walk");
			m_pNPC->MoveTowardsTarget();
		}
	}
	
	return E_STAYINSTATE;
}

//Exit: Does Nothing
void CRunState::Exit()
{
}