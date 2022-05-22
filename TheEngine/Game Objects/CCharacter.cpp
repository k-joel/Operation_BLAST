#include "CCharacter.h"
#include "../Collision/CCollisionTests.h"
#include "../Camera/Camera.h"
#include "../States/CGameState.h"

CCharacter::CCharacter()
{
	SetIsDead(false);
	SetCurrentHealth(5);
	SetMaxHealth(5);
	SetNumLives(3);
	m_bOnPlatform = false;
	m_pCurrentTarget = NULL;
}
CCharacter::~CCharacter()
{
}
/************************************************************************/
/* AI STUFF                                                             */
/************************************************************************/
void CCharacter::SelectTarget(list<CBase*> pTargets, bool bIsEnemy /* = true */)
{
	if(this->GetType() == OBJ_HELIOS)
	{
		(rand() % 2 == 0) ? m_pCurrentTarget = (CCharacter*)(*pTargets.begin()) : m_pCurrentTarget = (CCharacter*)(*pTargets.rbegin());
		return;
	}
	//if(this->GetIsOnGround() || GetIsOnPlatform())
	{
		float fBestResult = FLT_MAX;
		float fResult = 0.0f;
		for(list<CBase*>::iterator itr = pTargets.begin(); itr != pTargets.end(); ++itr)
		{
			//m_cCurTarget = NULL;
			bool bEnemyCheck = false;
			bool bSamePlane = FLOATEQUALS((*itr)->GetPosition().z,GetPosition().z,1.0f);
/*
			if(bIsEnemy)
				bEnemyCheck = !(*itr)->GetIsEnemy();
			else
				bEnemyCheck = (*itr)->GetIsEnemy() && ((*itr)->GetType() == OBJ_STRIKER || (*itr)->GetType() == OBJ_ASSAULTER);
*/

			if( (*itr) != this && (*itr)-GetIsActive() && bSamePlane && CCollisionTests::IntersectFrustrumSphere((*itr)->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()) == CT_IN)
			{
				fResult = fabs((*itr)->GetPosition().x - m_vPosition.x);
				if(fResult < fBestResult)
				{
					fBestResult			= fResult;
					m_pCurrentTarget	= (CCharacter*)(*itr);
				}
			}
		}
	}
}
bool CCharacter::GetInDistance(float fRadius)
{
	float fResult = 0.0f;
	if(m_pCurrentTarget && !m_pCurrentTarget->GetIsDead())
	{
		fResult = fabs(GetPosition().x  - m_pCurrentTarget->GetPosition().x);
		if(fResult <= fRadius)
			return true;
	}
	return false;
}
void CCharacter::MoveTowardsTarget()
{
	//Leave those commented out lines in, I may need them later.  Thanks! -TJ
	//if(this->GetIsOnGround() || GetIsOnPlatform())
	{
		if(GetCurrentTarget()->GetPosition().x <= GetPosition().x)
		{
			//SetVelocity(D3DXVECTOR3(-lua_tonumber((CGameState::GetInstance()->GetPlayerOne()->GetLuaState()), -5),GetVelocity().y,GetVelocity().z));
			//SetVelocity(D3DXVECTOR3(-50.0f,GetVelocity().y,GetVelocity().z));
			SetVelocity(-GetRunVelocity());
			SetRotationY(D3DX_PI*0.5);
			SetIsFacingLeft(true);
		}
		else if(GetCurrentTarget()->GetPosition().x >= GetPosition().x)
		{
			//SetVelocity(D3DXVECTOR3(lua_tonumber((CGameState::GetInstance()->GetPlayerOne()->GetLuaState()), -5),GetVelocity().y,GetVelocity().z));
			//SetVelocity(D3DXVECTOR3(50.0f,GetVelocity().y,GetVelocity().z));
			SetVelocity(GetRunVelocity());
			SetRotationY(-D3DX_PI*0.5);
			SetIsFacingLeft(false);
		}
	}
}