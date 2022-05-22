#pragma once

#include "CAssaulter.h"
#include "../Bullet/CMachineGunBullet.h"
#include "../Bullet/CShotgunBullet.h"
#include "../Bullet/CFlamethrowerBullet.h"
#include "../Bullet/CRocketBullet.h"
#include "../Bullet/CLightningBullet.h"
#include "../CPlayer.h"

#include "../../Mesh/modelImporter.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../../States/CGameState.h"
#include "../../Rendering/CRenderer.h"

#include "../../States/CGameState.h"
#include "../../States/CLoadState.h"

#include "../../FX/CFXManager.h"
#include "../../FX/CParticleManager.h"

#include "../../AI System/CState.h"

//const AkGameObjectID ENEMY_EXPLODE = 100;

CAssaulter::CAssaulter()
{
	m_nEnemyType = E_ASSAULTER;
	SetType(OBJ_ASSAULTER);
	m_bExplode = false;
	m_bCounted = false;
}

CAssaulter::~CAssaulter()
{
	m_mStateMachine.Shutdown();
}
void CAssaulter::Init(D3DXVECTOR3 vPosition, bool bDummyMode, int nHealth )
{
	pChargeParticle = NULL;
	m_bCounted = false;
	CEnemy::Init(vPosition);
	SetCurrentHealth((float)nHealth);
	SetIsActive(true);
	SetIsDead(false);
	m_bExplode = false;
	m_mStateMachine.Initialize(this,CObjectManager::GetInstance()->GetPlayerList(),bDummyMode);
	SetIsGalEnemy(false);
	SetIsLockEnemy(false);
	SetRenderType(RT_OPAQUE);
	if(bDummyMode)
	{
		SetRotationX(0);
		SetRotationY(0);
		SetRotationZ(0);
		SetVelocity(D3DXVECTOR3(0,0,0));
	}
}

void CAssaulter::Input()
{
}

void CAssaulter::Update(float fElapsedTime)
{
	if(GetPosition().z < 30.0f)
	{
		GetShape(0).SetTexID(GetShape(0).GetMesh()->GetTextureID());
	}
	else
	{
		GetShape(0).SetTexID(CLoadState::GetInstance()->GetImageID("Assaulter_color2"));
	}

	//for TJ
	D3DXMATRIX back = *GetShape(0).GetAnimCtrl().getMatrixByID(2) * GetFrame().GetWorldMatrix();

	GetShape(0).GetAnimCtrl().update(fElapsedTime);
	CEnemy::Update(fElapsedTime);
	m_mStateMachine.Update(fElapsedTime);
	CEnemy::Update(fElapsedTime);
	SetIsonPlatform(false);
}

void CAssaulter::Shutdown()
{
	CEnemy::Shutdown();
}

void CAssaulter::Render()
{
}
bool CAssaulter::CheckAngles()
{
	//X = (float)Math.Sin(angle);
	//Y = (float)Math.Cos(angle);

	if(GetCurrentTarget())
	{
			//Left Side
			D3DXVECTOR3 vNorm;
			D3DXVECTOR3 vTempor = GetCurrentTarget()->GetPosition() - GetPosition();
			D3DXVec3Normalize(&vNorm, &vTempor);

			float up	= cos(D3DXToRadian(60.0f));
			float y		= cos(vNorm.y);
			//float down	= cos(D3DXToRadian(315.0f));
			bool bUp = (vNorm.y < up);
			bool bDown = (vNorm.y > -up);
			if( bUp && bDown )
				return true;
	}
	return false;
}
bool CAssaulter::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}
void CAssaulter::CollisionReact(CBase *pBase)
{
	//The striker will ONLY handle ground collision and being hit by a bullet.
	//CBase::CollisionReact(pBase);
	//Returning if I'm already dead.
	if(GetIsDead())
		return;

	CEnemy::CollisionReact(pBase);

	switch(pBase->GetType())
	{
	case OBJ_MACHINE_GUN_BULLET:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer

				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}

				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);

				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_SHOTGUN_BULLET:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CShotgunBullet*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CShotgunBullet*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CShotgunBullet*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer
	
				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}

				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}

			break;
		}
	case OBJ_SHOTGUN_ALTERNATE:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CShotgunAlternate*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CShotgunAlternate*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CShotgunAlternate*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer

				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_SHOTGUN_ULTIMATE:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CShotgunUltimate*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CShotgunUltimate*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CShotgunUltimate*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer

				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_FLAME:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CFlamethrowerBullet*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CFlamethrowerBullet*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CFlamethrowerBullet*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer

				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_NAPALM:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CFlamethrowerAlternate*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CFlamethrowerAlternate*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CFlamethrowerAlternate*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer

				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_FLAME_ULTIMATE:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CFlamethrowerUltimate*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CFlamethrowerUltimate*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CFlamethrowerUltimate*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer

				D3DXMATRIX botMat = GetFrame().GetWorldMatrix();
				botMat._42 += 30.0f;

				if(	(((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->GetPlayerNumber() == 0))
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200b", botMat);
				}
				else
				{
					CParticleManager::GetInstance()->SpawnParticle("Plus_200", botMat);
				}
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_LIGHTNING:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CLightningBullet*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CLightningBullet*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CLightningBullet*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				//CFXManager::GetInstance()->LoadEffect(FX_EXPLOSION_BARREL, GetPosition(), 0.5f, false);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_LIGHTNING_ALTERNATE:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CLightningAlternate*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CLightningAlternate*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CLightningAlternate*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				//CFXManager::GetInstance()->LoadEffect(FX_EXPLOSION_BARREL, GetPosition(), 0.5f, false);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	case OBJ_LIGHTNING_ULTIMATE:
		{
			if(GetCurrentHealth() <= 0 && !GetIsDead())
			{
				((CPlayer*)(((CLightningUltimate*)(pBase))->GetOwner()))->IncreaseMultiplier(0.2f); //Increase the player's multiplier
				((CPlayer*)(((CLightningUltimate*)(pBase))->GetOwner()))->AddScore(ASSAULTER_SCORE); //Increase the player's score
				((CPlayer*)(((CLightningUltimate*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier timer
				if(GetIsLockEnemy())
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				SetIsDead(true);
				//CFXManager::GetInstance()->LoadEffect(FX_EXPLOSION_BARREL, GetPosition(), 0.5f, false);
				m_bAudioPlayDie = true;
				m_bExplode = true;
			}
			break;
		}
	}
}