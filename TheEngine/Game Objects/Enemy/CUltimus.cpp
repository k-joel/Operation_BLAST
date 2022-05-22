#pragma once

#include "CUltimus.h"
#include "../Bullet/CMachineGunBullet.h"
#include "../Bullet/CRocketBullet.h"
#include "../Bullet/CLaserBullet.h"
#include "../Bullet/CLightningBullet.h"
#include "../Bullet/CChargedLaser.h"
#include "../CPlayer.h"
#include "../CObjectFactory.h"
#include "../Scene/CFlag.h"
#include "../../Mesh/modelImporter.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../../States/CGameState.h"
#include "../../Rendering/CRenderer.h"
#include "../../Font/CBitMapFont.h"
#include "../CSpawner.h"

#include "../../fx/CParticleManager.h"

//const AkGameObjectID ENEMY_EXPLODE = 100;

#define DEBRIS_SPAWN 20.0f
#define CRATE_LIMIT 15.0f

CUltimus::CUltimus()
{
	m_nEnemyType = E_ULTIMUS;

	//CModelImporter::GetInstance()->LoadModel("Resources/Models/robot_geoShape.NNmesh", model);

	//D3DXMatrixIdentity(&m_mWorldPos);

	//model.duplicateControler(m_AnimCtrl);
	//m_BV.CloneAABB(&model.GetAABB());

	// Sound Stuff
	m_bPhaseOne = true;
	m_bPhaseTwo = false;
	m_bPhaseSan = false;
	m_bAtk1 = true;
	m_bAtk2 = false;
	m_bAtk3 = false;

	m_pTopCharge = NULL;
	m_pBottomCharge = NULL;
	m_pTopBlast = NULL;
	m_pBottomBlast = NULL;
	m_pGiantFUCKINGEXPLOSIONS = NULL;
	m_pSmoke0 = NULL;
	m_pSmoke1 = NULL;
	m_pSmoke2 = NULL;
	m_pSmoke3 = NULL;
	SetType(OBJ_ULTIMUS);

	this->m_bIsAffectedByGrav = false;

	m_eRainDebris = CEventSystem::GetInstance()->FindEvent("RainDebris");
	m_eStopDebris = CEventSystem::GetInstance()->FindEvent("StopDebris");
	// 	CRocketAlternate* bullet = (CRocketAlternate*)(CObjectFactory::GetInstance()->GetObject<CRocketAlternate>(OBJ_ROCKET_ALTERNATE));

	Register();
	m_fHitCounter = 0.0f;
	m_fDebrisTimer = 0.0f;
	m_fAudioSmashTimer = 0.0f;
}

CUltimus::~CUltimus()
{

	Deregister();
	m_mStateMachine.Shutdown();
}
void CUltimus::Init(D3DXVECTOR3 vPosition, bool bDummyMode, int nHealth )
{
	m_fHitCounter = 0.0f;
	CEnemy::Init(vPosition);

	m_pTopCharge = NULL;
	m_pBottomCharge = NULL;
	m_pTopBlast = NULL;
	m_pBottomBlast = NULL;

	m_pSmoke0 = NULL;
	m_pSmoke1 = NULL;
	m_pSmoke2 = NULL;
	m_pSmoke3 = NULL;
	m_pGiantFUCKINGEXPLOSIONS = NULL;

	SetCurrentHealth((float)nHealth);
	SetMaxHealth((float)nHealth);
	SetIsActive(true);
	SetIsDead(false);
	m_bPhaseOne = true;
	m_bPhaseTwo = false;
	m_bPhaseSan = false;
	m_bBossFightStart = false;

	m_bAtk1 = true;
	m_bAtk2 = false;
	m_bAtk3 = false;
	m_bAtk1Fire = false;
	m_bAtk2Fire = false;
	m_bAtk3Fire = false;
	if(bDummyMode)
	{
		SetRotationX(0);
		SetRotationY(0);
		SetRotationZ(0);
		SetVelocity(D3DXVECTOR3(0,0,0));
	}
	D3DXVECTOR3 temp = this->GetPosition();
	temp.y -= 55.0f;
	//temp.x -= 30.0f;

	m_pTopLaser		= (CBase*)(CObjectFactory::GetInstance()->GetObject<CBase>(OBJ_TOPLASER));
	m_pBottomLaser	= (CBase*)(CObjectFactory::GetInstance()->GetObject<CBase>(OBJ_BOTTOMLASER));
	m_pCannon		= (CBase*)(CObjectFactory::GetInstance()->GetObject<CBase>(OBJ_CANNON));
	m_pConveyor		= (CBase*)(CObjectFactory::GetInstance()->GetObject<CBase>(OBJ_CONVEYOR));
	m_pBody			= (CBase*)(CObjectFactory::GetInstance()->GetObject<CBase>(OBJ_BOSSBODY));
	m_pTopLaser->SetType(OBJ_TOPLASER);
	m_pBottomLaser->SetType(OBJ_BOTTOMLASER);
	m_pCannon->SetType(OBJ_CANNON);
	m_pConveyor->SetType(OBJ_CONVEYOR);
	m_pBody->SetType(OBJ_BOSSBODY);
	m_pTopLaser->SetIsAffectedByGrav(false);
	m_pBottomLaser->SetIsAffectedByGrav(false);
	m_pCannon->SetIsAffectedByGrav(false);
	m_pConveyor->SetIsAffectedByGrav(false);
	m_pBody->SetIsAffectedByGrav(false);
	this->SetIsAffectedByGrav(false);

	m_pTopLaser->SetPosition(temp);
	m_pBottomLaser->SetPosition(temp);
	m_pCannon->SetPosition(temp);
	m_pConveyor->SetPosition(temp.x,temp.y - 4.0f,temp.z);
	m_pBody->SetPosition(temp.x,temp.y,temp.z);
	m_pBody->GetBVAABB().GetAABB().minPt.x += temp.x;
	m_pBody->GetBVAABB().GetAABB().maxPt.x += temp.x;

	m_pTopLaser->GetShape(0).GetAnimCtrl().SetAnimation("topCannonIdle");
	m_pBottomLaser->GetShape(0).GetAnimCtrl().SetAnimation("bottomCannonIdle");
	m_pCannon->GetShape(0).GetAnimCtrl().SetAnimation("largeCannonIdle");
	m_pConveyor->GetShape(0).GetAnimCtrl().SetAnimation("conveyerIdle");

	CObjectManager::GetInstance()->AddObject(m_pTopLaser);
	CObjectManager::GetInstance()->AddObject(m_pBottomLaser);
	CObjectManager::GetInstance()->AddObject(m_pCannon);
	CObjectManager::GetInstance()->AddObject(m_pConveyor);
	CObjectManager::GetInstance()->AddObject(m_pBody);
	m_mStateMachine.Initialize(this,CObjectManager::GetInstance()->GetPlayerList(),bDummyMode);
	//SelectTarget(vTargets);
	m_fCrateTimer = 0.0f;
	m_bFirst = true;

}

void CUltimus::Input()
{
}
void CUltimus::Shutdown()
{
	m_pTopLaser->SetIsActive(false);
	m_pBottomLaser->SetIsActive(false);
	m_pCannon->SetIsActive(false);
	m_pConveyor->SetIsActive(false);
	m_pBody->SetIsActive(false);
	if(m_pGiantFUCKINGEXPLOSIONS)
		m_pGiantFUCKINGEXPLOSIONS->SetComplete(true);
	m_bBossFightStart = false;
	m_bFirst = true;


}
void CUltimus::Update(float fElapsedTime)
{
	//m_pConveyor->GetFrame().GetLocalMatrix() = *(GetShape(0).GetAnimCtrl().getMatrixByID(11));
	m_fHitCounter += fElapsedTime;
	static float fTimer = 0;
	D3DXMATRIX temp;// = GetBVAABB().GetAABB().
	D3DXMatrixTranslation(&temp,GetBVAABB().GetAABB().center.x,GetBVAABB().GetAABB().maxPt.y+50+50,GetBVAABB().GetAABB().center.z);
	if(GetIsDead())
	{
		if(!m_pGiantFUCKINGEXPLOSIONS)
		{
			m_pGiantFUCKINGEXPLOSIONS = CParticleManager::GetInstance()->SpawnParticle("BossSmoke",temp);
		}
		if(m_pGiantFUCKINGEXPLOSIONS)
			m_pGiantFUCKINGEXPLOSIONS->SetWorld(temp);
	}


	if(fTimer > 00.15f && GetIsDead())
	{
		fTimer = 0.0f;
		int random =  rand() % 4;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_DEATH, GetAudioID());
		switch(random)
		{
		case 0:
			{
				CParticleManager::GetInstance()->SpawnParticle("Explosion2",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(5) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
				CParticleManager::GetInstance()->SpawnParticle("BossExplosion",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(5) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
			}
			break;
		case 1:
			{
				CParticleManager::GetInstance()->SpawnParticle("Explosion2",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(6) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
				CParticleManager::GetInstance()->SpawnParticle("BossExplosion",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(6) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
			}
			break;
		case 2:
			{
				CParticleManager::GetInstance()->SpawnParticle("Explosion2",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(7) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
				CParticleManager::GetInstance()->SpawnParticle("BossExplosion",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(7) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
			}
			break;
		case 3:
			{
				CParticleManager::GetInstance()->SpawnParticle("Explosion2",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(8) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
				CParticleManager::GetInstance()->SpawnParticle("BossExplosion",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(8) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
			}
			break;
		}
	}

	if(GetCurrentHealth() <= 0)
	{
		if(m_pSmoke0)
		{
			m_pSmoke0->SetIsActive(false);
			m_pSmoke0 = NULL;
		}
		if(m_pSmoke1)
		{
			m_pSmoke1->SetIsActive(false);
			m_pSmoke1 = NULL;
		}
		if(m_pSmoke2)
		{
			m_pSmoke2->SetIsActive(false);
			m_pSmoke2 = NULL;
		}
		if(m_pSmoke3)
		{
			m_pSmoke3->SetIsActive(false);
			m_pSmoke3 = NULL;
		}
		SetIsDead(true);
	}
	if(GetCurrentHealth() <= 150 && !GetIsDead())
		if(!m_pSmoke0)
		m_pSmoke0 = CParticleManager::GetInstance()->SpawnParticle("BossSmallSmoke",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(5) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
	if(GetCurrentHealth() <= 100 && !GetIsDead())
		if(!m_pSmoke1)
			m_pSmoke1 = CParticleManager::GetInstance()->SpawnParticle("BossSmallSmoke",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(6) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
	if(GetCurrentHealth() <= 50 && !GetIsDead())
		if(!m_pSmoke2)
			m_pSmoke2 = CParticleManager::GetInstance()->SpawnParticle("BossSmallSmoke",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(7) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));
	if(GetCurrentHealth() <= 25 && !GetIsDead())
		if(!m_pSmoke3)
			m_pSmoke3 = CParticleManager::GetInstance()->SpawnParticle("BossSmallSmoke",(*m_pBody->GetShape(0).GetAnimCtrl().getMatrixByID(8) * m_pBody->GetShape(0).GetFrame().GetWorldMatrix()));

	GetShape(0).GetAnimCtrl().update(fElapsedTime);
	m_pTopLaser->GetShape(0).GetAnimCtrl().update(fElapsedTime);
	m_pBottomLaser->GetShape(0).GetAnimCtrl().update(fElapsedTime);
	m_pCannon->GetShape(0).GetAnimCtrl().update(fElapsedTime);
	m_pConveyor->GetShape(0).GetAnimCtrl().update(fElapsedTime);
	CBase::Update(fElapsedTime);
	CEnemy::Update(fElapsedTime);
	m_mStateMachine.Update(fElapsedTime);
	m_pBody->GetBVAABB().CreateMesh(m_pBody->GetBV());
	m_pBody->GetBVShape().SetMesh(m_pBody->GetBVAABB().GetMesh());
	//m_pBody->GetBVShape().GetRenderContext()->AddRenderNode(&m_pBody->GetBVShape());
	fTimer += fElapsedTime;

	if(m_bBossFightStart)
	{
		m_fCrateTimer += fElapsedTime;
		if(m_fCrateTimer > CRATE_LIMIT)
		{
			m_fCrateTimer = 0.0f;
			CSpawner::GetInstance()->RunScript("BossCrate", false, LOCK_WAVE);
		}
	}
}

void CUltimus::Render()
{
	// Comment out CBase::Render... use CRenderer for animations
	// use m_nAnimCtrl.SetAnimation() to change animations only
	//CBase::Render();
	int health = (int)GetCurrentHealth();
	char buffer[25];
	sprintf_s(buffer,sizeof(buffer),"B.O.S.S.:%i",(health >= 0)? health : 0);
	CBitMapFont::GetInstance()->Print(buffer,400,700,false,false,1.0);
}

bool CUltimus::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}
bool CUltimus::CheckSceneCollision(CBase* pBase)
{
	if(!GetIsDead())
		return CBase::CheckSceneCollision(pBase);
	else
		return false;
}
void CUltimus::CollisionReact(CBase *pBase)
{
	//The striker will ONLY handle ground collision and being hit by a bullet.
	//CBase::CollisionReact(pBase);
	//CEnemy::CollisionReact(pBase);
	if(m_fHitCounter > 0.1f )
	{
		switch(pBase->GetType())
		{
		case OBJ_MACHINE_GUN_BULLET:

		case OBJ_SHOTGUN_BULLET:
		case OBJ_SHOTGUN_ALTERNATE:
		case OBJ_SHOTGUN_ULTIMATE:

		case OBJ_FLAME:
		case OBJ_FLAME_ALTERNATE:
		case OBJ_FLAME_ULTIMATE:

		case OBJ_ROCKET_BULLET:
		case OBJ_ROCKET_ALTERNATE:
		case OBJ_ROCKET_ULTIMATE:

		case OBJ_LIGHTNING:
		case OBJ_LIGHTNING_ALTERNATE:
		case OBJ_LIGHTNING_ULTIMATE:
			{
				D3DXMATRIX robotCenter, scale;
				robotCenter = this->GetFrame().GetWorldMatrix();
				robotCenter._42 += 5.0f;
				robotCenter._41 -= 45.0f;
				//m_nHit = CParticleManager::GetInstance()->SpawnParticle("robothit",robotCenter);
				CParticleManager::GetInstance()->SpawnParticle("Robot_spark2", robotCenter);

				if(!m_bHit)
				{
				}
				m_fHitTimer = 0.50f;
				m_bHit = true;

				m_bAudioPlayHurt = true;
				m_fHitCounter = 0.0f;
				break;
			}
		}


		switch(pBase->GetType())
		{
		case OBJ_MACHINE_GUN_BULLET:
			{
				pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth()-((CMachineGunBullet*)(pBase))->GetDamage());
				//SetCurrentHealth(GetCurrentHealth()-190);//((CMachineGunBullet*)(pBase))->GetDamage());
				break;
			}
		case OBJ_SHOTGUN_BULLET:
			{
				pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth()-((CMachineGunBullet*)(pBase))->GetDamage());

				if(GetCurrentHealth() <= 0 && !GetIsDead())
				{
					((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->IncreaseMultiplier(0.1f); //Increase the er's multiplier
					((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier r
					SetIsDead(true);

				}

				break;
			}
		case OBJ_SHOTGUN_ALTERNATE:
			{
				pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth()-((CMachineGunBullet*)(pBase))->GetDamage());

				if(GetCurrentHealth() <= 0 && !GetIsDead())
				{
					((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->IncreaseMultiplier(0.1f); //Increase the er's multiplier
					((CPlayer*)(((CMachineGunBullet*)(pBase))->GetOwner()))->SetMultTimer(0.0f);  //Reset the multiplier r
					SetIsDead(true);

				}

				break;
			}
		case OBJ_FLAME:
			{
				pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth() - ((CMachineGunBullet*)(pBase))->GetDamage());
				break;
			}
		case OBJ_FLAME_ALTERNATE:
			{
				pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth()-((CMachineGunBullet*)(pBase))->GetDamage());
				//m_bAudioPlayHurt = true;
				break;
			}
		case OBJ_FLAME_ULTIMATE:
			{
				//pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth()-5);
				//m_bAudioPlayHurt = true;
				break;
			}
		case OBJ_GROUND:
			{
				//Ground collision stuff
				break;
			}
		case OBJ_SHOTGUN_ULTIMATE:
			{
				pBase->SetIsActive(false);
				if(m_bBossFightStart)
					SetCurrentHealth(GetCurrentHealth()-((CMachineGunBullet*)(pBase))->GetDamage());
				//m_bAudioPlayHurt = true;
				break;
			}
		}
	}
}

void CUltimus::Register()
{
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eRainDebris, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eStopDebris, 1.0f);
}

void CUltimus::React(const tDispatch &toReactTo)
{
	char buffer[16];
	strcpy_s(buffer, 16, ((tDispatch*)(&toReactTo))->m_pEvent->GetEventName());

	if(strcmp(buffer, "RainDebris") == 0)
	{
		//Not sure if the Ultimus should actually be reacting to the debris or not.
	}

	if(strcmp(buffer, "StopDebris") == 0)
	{
		//Not sure if the Ultimus should actually be reacting to the debris or not.
	}
}

void CUltimus::Deregister()
{
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eRainDebris);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eStopDebris);
}