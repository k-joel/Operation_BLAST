#include "CState.h"
#include "../Game Objects/Enemy/CHelios.h"
#include "../Camera/Camera.h"
#include "../Collision/CCollisionTests.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

float RandomFloat(float min, float max);

//Enter: Picks a random target then flys towards it
void	CFlyState::Enter()
{
	if(!((CHelios*)m_pNPC)->GetIsFlyingAway())
	{
		if( ((CEnemy*)m_pNPC)->GetPlayFly() == false)
		{
 			((CEnemy*)m_pNPC)->SetPlayFly(true);
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_HELICOPTER_LOOP, (AkGameObjectID)((CHelios*)m_pNPC)->GetAudioID());
		}
		m_pNPC->SelectTarget(m_pTargets);
		m_vToPlayer = CCamera::GetInstance()->GetViewPosition() - m_pNPC->GetPosition();
		if(m_vToPlayer.x > 0.0f)
		{
			m_vToPlayer.x += -75.0f;
			m_pNPC->SetRotationY(D3DX_PI * -0.5f);
		}
		else
		{
			m_vToPlayer.x += 75.0f;
			((CHelios*)m_pNPC)->SetIsLeft(true);
			m_pNPC->SetRotationY(D3DX_PI * 0.5f);
		}

		D3DXVec3Normalize(&m_vNormToPlayer,&m_vToPlayer);
	}
	m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("startMove");
}

//GetUtility: Checks various conditions, if it succeeded it returns a value, if not it returns a ZERO
float CFlyState::GetUtility()
{
	if(((CHelios*)m_pNPC)->GetIsFlyingAway())
			return 1.0f;
	return 0.0f;
}

//Update: Just Flys 
int		CFlyState::Update(float fElapsedTime)
{
	if(m_pNPC->GetShape(0).GetAnimCtrl().getLoopCount())
		m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("move");

	float x = 0.0f;
	float y = 0.0f;

	if(m_vNormToPlayer.x < 0.0f)
	{
		x = -1.0f;
		((CHelios*)m_pNPC)->SetIsLeft(true);
	}
	else if(m_vNormToPlayer.x > 0.0f)
	{
		x = 1.0f;
		((CHelios*)m_pNPC)->SetIsLeft(false);
	}
	if(m_vNormToPlayer.y < 0.0f)
		y = -1.0f;
	else if(m_vNormToPlayer.y > 0.0f)
		y = 1.0f;

	if(((CHelios*)m_pNPC)->GetIsLeft())
		m_pNPC->SetVelocity(-m_pNPC->GetRunVelocity());
	else
		m_pNPC->SetVelocity(m_pNPC->GetRunVelocity());

	m_pNPC->SetPosition(m_pNPC->GetPosition().x, (sin(m_pNPC->GetPosition().x* 0.1f ) * 5.0f ) + CCamera::GetInstance()->GetViewPosition().y+15,m_pNPC->GetPosition().z);


	return E_STAYINSTATE;
}

//Exit: Does Nothing
void	CFlyState::Exit()
{
}