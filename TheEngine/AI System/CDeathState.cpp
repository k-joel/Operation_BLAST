#include "CState.h"
#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../Game Objects/Enemy/CHelios.h"
#include "../Game Objects/Enemy/CUltimus.h"
#include "../Camera/Camera.h"
#include "../Collision/CCollisionTests.h"

//Enter: Sets the enmey to stop moving and sets its death animation
void CDeathState::Enter()
{
	m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("die");
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_WEEBOS_HELICOPTER_LOOP, ((CEnemy*)m_pNPC)->GetAudioID());
	if(m_pNPC->GetType() == OBJ_ULTIMUS)
		((CUltimus*)m_pNPC)->SetPlayBossDie(true);

	m_pNPC->SetVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_fSpawnTimer = 0.0f;
}
float	CDeathState::GetUtility(void )
{
	if(m_pNPC->GetIsDead() || m_pNPC->GetPosition().y == -200.0f || 
		( m_pNPC->GetType() == OBJ_HELIOS && ((CHelios*)m_pNPC)->GetIsFlyingAway() && CCollisionTests::IntersectFrustrumSphere(m_pNPC->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()) == CT_OUT))
		return 1.1f;
	return 0.0f;
}

//Update: Goes and updates a timer to keep them around to actually play the animation and then sets the enemy to not active
int CDeathState::Update(float fElapsedTime)
{
	static float fTimer = 0;
	fTimer += fElapsedTime;
	m_fSpawnTimer += fElapsedTime;

	if(m_pNPC->GetType() != OBJ_ULTIMUS)
	{
		if(m_fSpawnTimer >= 0.5f)
		{
			((CEnemy*)m_pNPC)->SetPlayFly(false);
			m_pNPC->SetIsActive(false);
		}
	}
	else
	{
		if(m_fSpawnTimer >= 8.5f)
		{
			m_pNPC->SetIsActive(false);
		}
		else
		{
			if(fTimer > 0.05)
			{
				fTimer = 0;
				m_pNPC->SetPosition(m_pNPC->GetPosition().x,m_pNPC->GetPosition().y - 3,m_pNPC->GetPosition().z);
				for(unsigned int i(0); i < 5;++i)
					((CUltimus*)m_pNPC)->GetPart(i)->SetPosition(((CUltimus*)m_pNPC)->GetPart(i)->GetPosition().x,((CUltimus*)m_pNPC)->GetPart(i)->GetPosition().y - 3,((CUltimus*)m_pNPC)->GetPart(i)->GetPosition().z);
			}
		}

	}
	return E_STAYINSTATE;
}

//Exit: Does Nothing
void CDeathState::Exit()
{
}