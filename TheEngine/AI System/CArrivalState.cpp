#ifndef CArrivalState_h__
#define CArrivalState_h__
#include "CState.h"
#include "../Game Objects/Enemy/CEnemy.h"
#include "../Camera/Camera.h"
#include "../FX/CParticleManager.h"

//Enter: It gets the enemy's Spawn method and choices where the enemy spawns
void	CArrivalState::Enter()
{
	switch(((CEnemy*)m_pNPC)->GetSpawnMethod())
	{
	case DEFAULT_METHOD:
		((CEnemy*)m_pNPC)->SetIsArriving(false);
		break;
	case JUMP_IN_METHOD:
		{
			m_vJumpTarget = m_pNPC->GetPosition();
			D3DXVECTOR3 startPos = CCamera::GetInstance()->GetViewPosition();
			startPos.z = -300.0f;
			startPos.x = startPos.x - (rand() % 60 - 30.0f);
			startPos.y = m_vJumpTarget.y;
			m_pNPC->SetPosition(startPos);
			m_vJumpDir = m_vJumpTarget - startPos;
			D3DXVec3Normalize(&m_vJumpDir, &m_vJumpDir);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jump");
			m_pNPC->SetVelocity(0.0f, 275.0f, 0.0f);
			m_pNPC->SetRotationY(D3DXToRadian(180.0f));
			((CEnemy*)m_pNPC)->SetIsArriving(true);

			//Would really like code here to make them face the direction they're jumping.
			break;
		}
	case DROP_DOWN_METHOD:
		{
			((CEnemy*)m_pNPC)->SetIsArriving(true);

			m_vDropPos = m_pNPC->GetPosition();
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jump");
			D3DXVECTOR3 tempPos = m_vDropPos;
			tempPos.y += 230.0f;
			m_pNPC->SetPosition(tempPos);
			m_pNPC->SetRotationY(D3DXToRadian(90.0f));
			m_fDropTimer = 0.0f;
			D3DXMATRIX locMat;
			D3DXMatrixIdentity(&locMat);
			locMat._41 = -100;//tempPos.x;
			locMat._42 = 60;//tempPos.y;
			locMat._43 = 0;//tempPos.z;

			//CParticleManager::GetInstance()->SpawnParticle("Leaf_flutter2", locMat);
			//Need to set a particle here or some sort of feedback warning.  So the player knows something is incomming.
			break;
		}
	case TREE_METHOD:
		{
			((CEnemy*)m_pNPC)->SetIsArriving(true);

			D3DXMATRIX partMat;
			D3DXMatrixIdentity(&partMat);
			partMat._41 = m_pNPC->GetPosition().x;
			partMat._42 = m_pNPC->GetPosition().y;
			partMat._43 = m_pNPC->GetPosition().z;
			m_fTreeTimer = 0.0f;
			m_vTreePos = m_pNPC->GetPosition();
			m_pNPC->SetRenderType(RT_DONOTRENDER);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jump");
			CParticleManager::GetInstance()->SpawnParticle("Leaf_flutter2", partMat);
			break;
		}
	case BOSS_METHOD:
		{
			((CEnemy*)m_pNPC)->SetIsArriving(true);

			m_vBossWalkTarget = m_pNPC->GetPosition();
			m_vBossWalkTarget.x -= 54.f;  //THIS WILL MORE THAN LIKELY NEED TO BE TWEAKED.
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("walk");
			m_pNPC->SetRotationY(D3DXToRadian(90.0f));
			m_bBossTargetArrived = false;
			break;
		}
	case JETPACK_METHOD:
		{
			((CEnemy*)m_pNPC)->SetIsArriving(true);

			m_pNPC->SetIsOnGround(false);
			m_bLanded = false;
			m_fLandingTimer = 0.0f;
			D3DXVECTOR3 piggityPos = CCamera::GetInstance()->GetViewPosition();
			piggityPos.y += 200.0f;
			piggityPos.z = m_pNPC->GetPosition().z;
			piggityPos.x = m_pNPC->GetPosition().x;
			m_pNPC->SetRotationY(90);
			m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jetpack");
			m_pNPC->SetPosition(piggityPos);
			m_pNPC->SetVelocity(0.0f, 0.0f, 0.0f);
			D3DXMatrixIdentity(&m_mJetpack);
			m_mJetpack = *m_pNPC->GetShape(0).GetAnimCtrl().getMatrixByID(2) * m_pNPC->GetFrame().GetWorldMatrix();
			m_mJetpack._41 += 4.0f;
			m_pJetpack = CParticleManager::GetInstance()->SpawnParticle("Assaulter_jetpack", m_mJetpack);
			break;
		}
	case TELEPORT_METHOD:
		{
			break;
		}
	}
}

//Update: Constantly updates the enemy until it hits its end goal
int	CArrivalState::Update(float fElapsedTime)
{
	// At the End of all of these you will need to return E_GOTOSELECT when you are at your goal
	// The E_STAYINSTATE makes them state in the state until you return something different

	switch(((CEnemy*)m_pNPC)->GetSpawnMethod())
	{
	case DEFAULT_METHOD:
		return E_GOTOSELECT;
		break;
	case JUMP_IN_METHOD:
		{
			//This if-check sees if I'm not in the front plane.  So keep on a-jumpin'.
			if(m_pNPC->GetPosition().z < m_vJumpTarget.z)
			{
				D3DXVECTOR3 newPos = m_pNPC->GetPosition();
				if(m_vJumpTarget.z > 30.0f)
				{
					newPos.z += m_vJumpDir.z * 550.0f * fElapsedTime;
					newPos.x += m_vJumpDir.x * 550.0f * fElapsedTime;
				}
				else
				{
					newPos.z += m_vJumpDir.z * 350.0f * fElapsedTime;
					newPos.x += m_vJumpDir.x * 350.0f * fElapsedTime;
				}

				m_pNPC->SetPosition(newPos);
			}
			else
			{
				D3DXVECTOR3 newPos = m_pNPC->GetPosition();

				if(m_vJumpTarget.z > 30.0f)
					newPos.z = 60.0f;
				else
					newPos.z = 0.0f;
				m_pNPC->SetPosition(newPos);
				((CEnemy*)m_pNPC)->SetIsArriving(false);

				return E_GOTOSELECT;
			}
			break;
		}
	case DROP_DOWN_METHOD:
		{
			if(m_fDropTimer >= 1.0f)
			{
				if(!m_pNPC->GetIsOnGround())
				{
				}
				else
				{
					((CEnemy*)m_pNPC)->SetIsArriving(false);

					return E_GOTOSELECT;
				}
			}
			else
			{
				m_fDropTimer += fElapsedTime;
				m_pNPC->SetVelocity(0.0f, 0.0f, 0.0f);
			}
			break;
		}
	case TREE_METHOD:
		{
			m_fTreeTimer += fElapsedTime;

			if(m_fTreeTimer <= 3.0f)
			{
				m_pNPC->SetPosition(m_vTreePos);
			}
			else
			{
				m_pNPC->SetRenderType(RT_OPAQUE);
				if(!m_pNPC->GetIsOnGround())
				{
				}
				else
				{
					((CEnemy*)m_pNPC)->SetIsArriving(false);

					return E_GOTOSELECT;
				}
			}
			break;
		}
	case BOSS_METHOD:
		{
			if(m_pNPC->GetPosition().z < 0.0f)
			{
				if(!m_bBossTargetArrived)
				{
					m_pNPC->SetVelocity(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 tempPos = m_pNPC->GetPosition();
					tempPos.x -= m_pNPC->GetRunVelocity().x * fElapsedTime;
					m_pNPC->SetPosition(tempPos);

					if(m_pNPC->GetPosition().x <= m_vBossWalkTarget.x)
					{
						m_bBossTargetArrived = true;
						m_pNPC->SetRotationY(D3DXToRadian(180.0f));
					}
				}
				else
				{
					if(m_pNPC->GetIsOnGround())
					{
						m_pNPC->SetVelocity(0.0f, 0.0f, 0.0f);
						D3DXVECTOR3 tempPos = m_pNPC->GetPosition();
						tempPos.z += m_pNPC->GetRunVelocity().x * fElapsedTime;
						m_pNPC->SetPosition(tempPos);
					}
				}
			}
			else
			{
				((CEnemy*)m_pNPC)->SetIsArriving(false);

				return E_GOTOSELECT;
			}

			break;
		}
	case JETPACK_METHOD:
		{
			if(!m_bLanded)
			{
				m_mJetpack = *m_pNPC->GetShape(0).GetAnimCtrl().getMatrixByID(2) * m_pNPC->GetFrame().GetWorldMatrix();
				m_mJetpack._41 += 4.0f;
				m_pJetpack->SetWorld(m_mJetpack);

				if(m_pNPC->GetVelocity().y < -50.0f)
					m_pNPC->SetVelocity(m_pNPC->GetVelocity().x, -50.0f, m_pNPC->GetVelocity().z);

				if(m_pNPC->GetIsOnGround())
				{
					CParticleManager::GetInstance()->SpawnParticle("Assaulter_land", m_pNPC->GetFrame().GetWorldMatrix());
					m_bLanded = true;
					m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("jetpackLand");
				}

			}
			else
			{
				//m_pJetpack->SetIsActive(false);
				for(unsigned int x = 0; x < m_pJetpack->GetEmitterList().size(); ++x)
				{
					m_pJetpack->GetEmitterList()[x]->m_bIsCompleted = true;
					m_pJetpack->GetEmitterList()[x]->m_bKillOnCompleted = true;
				}

				if(m_fLandingTimer > 1.08f)
				{
					((CEnemy*)m_pNPC)->SetIsArriving(false);
					return E_GOTOSELECT;
				}

				m_fLandingTimer += fElapsedTime;
			}

			break;
		}
	case TELEPORT_METHOD:
		{
			break;
		}
	}
	return E_STAYINSTATE;
}
//Exit: If any particles were assigned to the enemies they get killed here
void	CArrivalState::Exit()
{		
	if(((CEnemy*)m_pNPC)->GetSpawnMethod() == JETPACK_METHOD && !m_bLanded )
	{
		m_pJetpack->SetIsActive(false);
	}
}

#endif // CArrivalState_h__