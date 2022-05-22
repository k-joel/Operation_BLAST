#include "CState.h"
#include "../Game Objects/Bullet/CLaserBullet.h"
#include "../Game Objects/CObjectFactory.h"
#include "../Game Objects/CObjectManager.h"
#include "../Game Objects/Enemy/CHelios.h"
#include "../Game Objects/Enemy/CAssaulter.h"
#include "../Collision/CCollisionTests.h"
#include "../Camera/Camera.h"
#include "../../fx/CParticleManager.h"

float RandomFloat(float min, float max);

D3DXVECTOR3 vToPlayer;
D3DXVECTOR3 vTemp;
bool bTester = false;

D3DXMATRIX BlasterMat;
//Enter: Creates the vector to its target
void CShootState::Enter()
{
	vToPlayer = m_pNPC->GetCurrentTarget()->GetPosition() - m_pNPC->GetPosition();
	D3DXVec3Normalize(&vTemp,&vToPlayer);
	bTester = false;

	if(m_pNPC->GetType() == OBJ_HELIOS)
	{
		m_pNPC->SetVelocity(0,0,0);

		m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("stopMove");
	}
	else
	{
		IbeChargin = false;
		pEffect = NULL;
		nAnimLoop = 0;
	}
}
//GetUtility: Checks various conditions, if it succeeded it returns a value, if not it returns a ZERO
float CShootState::GetUtility()
{
	if( (m_pNPC->GetType() == OBJ_ASSAULTER  && m_pNPC->GetInDistance(150.0f) && m_pNPC->GetIsOnGround() && ((CAssaulter*)m_pNPC)->CheckAngles() &&(CCollisionTests::IntersectFrustrumSphere2(m_pNPC->GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum()) == CT_IN) ))
		return 0.98f;
		if( m_pNPC->GetType() == OBJ_HELIOS && !((CHelios*)m_pNPC)->GetIsFlyingAway() && FLOATEQUALS(m_pNPC->GetPosition().x,RandomFloat(m_pNPC->GetCurrentTarget()->GetPosition().x,m_pNPC->GetCurrentTarget()->GetPosition().x),25.0f))
			return 0.98f;
	return 0.0f;
}
//Update: Shoots at the target if it has one
int CShootState::Update(float fElapsedTime)
{
		static float fTimer = 0.0f;
		fTimer += fElapsedTime;

		if(m_pNPC->GetType() != OBJ_HELIOS)
		{
			if(IbeChargin)
				if(!pEffect)
				{
					D3DXMATRIX BlasterMat = *(m_pNPC->GetShape(0).GetAnimCtrl().getMatrixByID(1)) * m_pNPC->GetFrame().GetWorldMatrix();
					pEffect = CParticleManager::GetInstance()->SpawnParticle("Laser_charge", BlasterMat);
				}

			if(m_pNPC->GetCurrentTarget()->GetPosition().x <= m_pNPC->GetPosition().x)
			{
				m_pNPC->SetRotationY(D3DX_PI*0.5);
				m_pNPC->SetIsFacingLeft(true);
			}
			else if(m_pNPC->GetCurrentTarget()->GetPosition().x >= m_pNPC->GetPosition().x)
			{
				m_pNPC->SetRotationY(-D3DX_PI*0.5);
				m_pNPC->SetIsFacingLeft(false);
			}

			if(m_pNPC->GetShape(0).GetAnimCtrl().getLoopCount() && !bTester)
			{

				switch(nAnimLoop)
				{
				case 0:
					{
						m_pNPC->SetVelocity(0,0,0);
						IbeChargin = true;
						m_pNPC->GetShape(0).GetAnimCtrl().BlendAnimation("shootIdle",0.1f);
						m_pNPC->GetShape(0).GetAnimCtrl().reset();

						nNum++;
					}
					break;
				case 1:
					{
						IbeChargin = false;
						if(pEffect)
						{
							pEffect->SetIsActive(false);
							pEffect = NULL;
						}

						nNum = 0;
						m_pNPC->SetVelocity(0,0,0);
						
						m_pNPC->GetShape(0).GetAnimCtrl().BlendAnimation("shoot",0.1f);

						vToPlayer = m_pNPC->GetCurrentTarget()->GetPosition() - m_pNPC->GetPosition();
						D3DXVec3Normalize(&vTemp,&vToPlayer);

						((CEnemy*)m_pNPC)->SetPlayShoot(true);
						CLaserBullet* bullet = (CLaserBullet*)(CObjectFactory::GetInstance()->GetObject<CLaserBullet>(OBJ_LASER_BULLET));
						D3DXVECTOR3 tempPos = m_pNPC->GetPosition();
						tempPos.y += 17.0f;
						bullet->SetPosition(tempPos);
						bullet->SetIsActive(true);
						bullet->SetOwner(m_pNPC);
						bullet->SetType(OBJ_LASER_BULLET);

						D3DXVECTOR3 vVelocity = D3DXVECTOR3(100.0f,100.0f,0.0f);
						vVelocity = D3DXVECTOR3(vVelocity.x * vTemp.x,vVelocity.y * vTemp.y,vVelocity.z * vTemp.z);
						bullet->SetRotationZ(D3DX_PI * vTemp.x);
						bullet->SetVelocity(vVelocity);
						((CEnemy*)m_pNPC)->SetPlayShoot(true);
						CObjectManager::GetInstance()->AddObject(bullet);
					}
					break;
				case 2:
					{
						IbeChargin = false;

						m_pNPC->SetVelocity(0,0,0);
						m_pNPC->GetShape(0).GetAnimCtrl().BlendAnimation("shootIdle",0.1f);
						nAnimLoop = 0;
					}
					break;
				}
				nAnimLoop++;
			}
		}
		else
		{
			if(m_pNPC->GetShape(0).GetAnimCtrl().getLoopCount())
			{
				switch(nAnimLoop)
				{
				case 0:
					{
						if(((CHelios*)m_pNPC)->GetIsLeft())
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("turnLeft");
						else
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("turnRight");
					}
					break;
				case 1:
					{
						if(((CHelios*)m_pNPC)->GetIsLeft())
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("leftReadyShoot");
						else
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("rightReadyShoot");
					}
					break;
				case 2:
					{
						if(((CHelios*)m_pNPC)->GetIsLeft())
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("leftShoot");
						else
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("rightShoot");

						CLaserBullet* bullet = (CLaserBullet*)(CObjectFactory::GetInstance()->GetObject<CLaserBullet>(OBJ_LASER_BULLET));
						bullet->SetPosition(m_pNPC->GetPosition());
						bullet->SetIsActive(true);
						bullet->SetOwner(m_pNPC);
						bullet->SetType(OBJ_LASER_BULLET);

						D3DXVECTOR3 vVelocity = D3DXVECTOR3(0.0f,-100.0f,0.0f);
						vVelocity = D3DXVECTOR3(vVelocity.x,vVelocity.y,vVelocity.z);
						bullet->SetVelocity(vVelocity);
						bullet->SetRotationZ(D3DX_PI * 0.5f);
						((CEnemy*)m_pNPC)->SetPlayShoot(true);
						CObjectManager::GetInstance()->AddObject(bullet);
						m_pNPC->GetShape(0).GetAnimCtrl().reset();
						if(!(nNum > 1))
							nAnimLoop = 1;
						nNum++;
					}
					break;
				case 3:
					{
						if(((CHelios*)m_pNPC)->GetIsLeft())
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("unturnLeft");
						else
							m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("unturnRight");
					}
					break;
				case 4:
					{
						((CHelios*)m_pNPC)->SetIsFlyingAway(true);

					}
					break;
				case 5:
					{
						m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("move");
					}
					break;
				case 6:
					{
						m_pNPC->GetShape(0).GetAnimCtrl().SetAnimation("stopMove");
						nAnimLoop = -1;
						m_pNPC->SetVelocity(0.0f,0.0f,0.0f);
					}
					break;
				}
				nAnimLoop++;
			}
			return E_STAYINSTATE;
		}
		return E_STAYINSTATE;
}

//Exit: Deletes Particles
void CShootState::Exit()
{
	IbeChargin = false;
	if(m_pNPC->GetType() == OBJ_ASSAULTER)
		if(pEffect)
	{
		pEffect->SetIsActive(false);
		pEffect = NULL;
	}
	nNum = 0;
}