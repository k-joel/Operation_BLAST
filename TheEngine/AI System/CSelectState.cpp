#include "CState.h"
#include "../Game Objects/CObjectManager.h"

//Enter: Calls the enemy's select a target
void CSelectState::Enter()
{

		m_pNPC->SelectTarget(m_pTargets);
		if(m_pNPC->GetType() == OBJ_ASSAULTER)
		{
			m_pNPC->SetVelocity(0,0,0);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("idle");
		}

}
//GetUtility: Checks various conditions, if it succeeded it returns a value, if not it returns a ZERO
float	CSelectState::GetUtility(void )
{
	int nEnemyCounter = 0;
	float fResult = 0.0f;
	float fBestResult = FLT_MAX;
	CBase* pBest = NULL;
	for(list<CBase*>::iterator i = CObjectManager::GetInstance()->GetPlayerList().begin(); i != CObjectManager::GetInstance()->GetPlayerList().end();++i )
	{
		if(CCollisionTests::IntersectFrustrumSphere((*i)->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()))
			nEnemyCounter++;

		//////////////////////////////////////////////////////////////////////////

		//if(m_pNPC->GetCurrentTarget())
		{
			bool bSamePlane = FLOATEQUALS((*i)->GetPosition().z,m_pNPC->GetPosition().z,2.0f);

			if( bSamePlane )
			{
				fResult = fabs((*i)->GetPosition().x - m_pNPC->GetPosition().x);
				if(fResult < fBestResult)
				{
					fBestResult		= fResult;
					pBest			= (*i);
				}
			}
		}
	}
	return (((!m_pNPC->GetCurrentTarget() && nEnemyCounter > 0) || (pBest && ((CCharacter*)pBest) != m_pNPC->GetCurrentTarget())) && !((CEnemy*)m_pNPC)->GetIsArriving() && (m_pNPC->GetIsOnGround() || m_pNPC->GetIsOnPlatform()))  ? 1.0f : 0.0f;
}

//Update: If the enemy has no target is continues call the selecttarget
int CSelectState::Update(float fElapsedTime)
{
/*

	if(m_pNPC->GetCurrentTarget())
		return E_GOTORUN;
	if(m_pNPC->GetIsDead())
		return E_GOTODEATH;*/


	Enter();

	return E_STAYINSTATE;
}


//Exit: DOes Nothing
void CSelectState::Exit()
{

}
