#ifndef CCoopSelect_h__
#define CCoopSelect_h__

#include "../CState.h"
#include "../../Camera/Camera.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Game Objects/CObjectManager.h"

void	CCoopSelect::Enter()
{
	m_pNPC->SelectTarget(CObjectManager::GetInstance()->GetPlayerList(),false);
}

int	CCoopSelect::Update(float fElapsedTime)
{
	m_pNPC->SelectTarget(CObjectManager::GetInstance()->GetPlayerList(),false);
	return 0;
}
float	CCoopSelect::GetUtility(void )
{
	int nEnemyCounter = 0;
	float fResult = 0.0f;
	float fBestResult = FLT_MAX;
	CBase* pBest = NULL;
	for(list<CBase*>::iterator i = CObjectManager::GetInstance()->GetEnemyList().begin(); i != CObjectManager::GetInstance()->GetEnemyList().end();++i )
	{
		if(CCollisionTests::IntersectFrustrumSphere((*i)->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()))
			nEnemyCounter++;

		//////////////////////////////////////////////////////////////////////////

		if(m_pNPC->GetCurrentTarget())
		{
			bool bSamePlane = (*i)->GetPosition().z == m_pNPC->GetPosition().z;

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

/*
	if(m_pTargets.size() == 0)
		return 0.0f;
*/

	return (!m_pNPC->GetCurrentTarget() && nEnemyCounter > 0) || (((CCharacter*)pBest) != m_pNPC->GetCurrentTarget()) ? 1.0f : 0.0f;
}
void	CCoopSelect::Exit()
{
}

#endif // CCoopSelect_h__