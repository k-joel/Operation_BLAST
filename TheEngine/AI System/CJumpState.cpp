#include "CState.h"
#include "../Game Objects/CPlayer.h"
#include "../Game Objects/CBase.h"

//Enter: Does Nothing
void	CJumpState::Enter()
{
}
//GetUtility: Checks various conditions, if it succeeded it returns a value, if not it returns a ZERO
float CJumpState::GetUtility()
{

	if( m_pNPC->GetCurrentTarget() && (
		(m_pNPC->GetType() == OBJ_STRIKER && m_pNPC->GetCurrentTarget()->GetIsOnPlatform() && !m_pNPC->GetIsOnPlatform() && m_pNPC->GetInDistance(75.0f))
		|| 
		( ((CEnemy*)m_pNPC)->GetJumpPadTime() && m_pNPC->GetCurrentTarget()->GetPosition().x < m_pNPC->GetPosition().x) )
	  )
	  return 0.87f;
	return 0.0f;
}
//Update: Makes the enemies jump
int	CJumpState::Update(float fElapsedTime)
{
	static float fTimer = 0.0f;
	fTimer	+= fElapsedTime;


	{

		const float fJumpStr = 175.0f;

		if(((CEnemy*)m_pNPC)->GetJumpPadTime())
		{
			((CEnemy*)m_pNPC)->SetJumpPadTime(false);
			fTimer = 0.0f;
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jump");
			if(m_pNPC->GetIsOnGround())
			{
				m_pNPC->SetPosition(D3DXVECTOR3(m_pNPC->GetPosition().x,m_pNPC->GetPosition().y + 5.0f + JUMP_EPSILON,m_pNPC->GetPosition().z));
				m_pNPC->SetVelocity(D3DXVECTOR3(-100.0f,fJumpStr,0.0f));
			}
			if(((CEnemy*)m_pNPC)->GetPlayJump() == false)
				((CEnemy*)m_pNPC)->SetPlayJump(true);
		}
		else if(m_pNPC->GetCurrentTarget()->GetIsOnPlatform() && !m_pNPC->GetIsOnPlatform() && m_pNPC->GetInDistance(75.0f)  )//&& !((CEnemy*)m_pNPC)->GetIsJumping() )
		{
			((CEnemy*)m_pNPC)->SetJumpPadTime(false);
			fTimer = 0.0f;
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jump");
			if(m_pNPC->GetIsOnGround())
			{
				m_pNPC->SetPosition(D3DXVECTOR3(m_pNPC->GetPosition().x,m_pNPC->GetPosition().y + 5.0f + JUMP_EPSILON,m_pNPC->GetPosition().z));
				m_pNPC->SetVelocity(D3DXVECTOR3(m_pNPC->GetVelocity().x,fJumpStr,0.0f));
			}
			if(((CEnemy*)m_pNPC)->GetPlayJump() == false)
				((CEnemy*)m_pNPC)->SetPlayJump(true);
		}

		else if(m_pNPC->GetIsOnPlatform() ||  m_pNPC->GetIsOnGround())
		{
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("walk");
			return E_GOTORUN;
		}


	}

	return E_STAYINSTATE;
}

//Exit: Does Nothing
void	CJumpState::Exit()
{

}