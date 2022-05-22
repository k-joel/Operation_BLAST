#include "CEnemy.h"
#include "../../States/CGameState.h"
#include "../../Camera/Camera.h"

#include "../../FX/CFXManager.h"

//Includes for the various weapon types
#include "../Bullet/CMachineGunBullet.h"
#include "../Bullet/CLightningBullet.h"
#include "../Bullet/CShotgunBullet.h"
#include "../Bullet/CRocketBullet.h"
#include "../Bullet/CFlamethrowerBullet.h"
#include "../../Rendering/CRenderContext.h"
#include "../CObjectManager.h"
#include "../../Collision/CCollisionTests.h"
#include "../../FX/CParticleManager.h"

const AkGameObjectID ENEMY = 3;
long long CEnemy::ID = 210;

float RandomFloat(float min, float max)
{
	return (rand() / ((float)RAND_MAX + 1.0f)) * (max - min) + min;
}

void CEnemy::Initialize(void)
{
	m_bShotgunHit = false;
}

CEnemy::CEnemy()
{
	SetIsDead(false);
	SetIsEnemy(true);
	SetCurrentHealth(3);
	SetMaxHealth(3);
	SetNumLives(0);
	SetType(OBJ_ENEMY);
	SetPosition(D3DXVECTOR3(-1000.0f,-1000.0f,-1000.0f));

	m_bExplode			=	false;
	m_bJumping			=	false;
	m_bJumpPadTime		=	false;
	m_bAudioPlayJump	=	false;
	m_bAudioPlayLand	=	false;
	m_bAudioPlayDie		=	false;
	m_bAudioPlayHurt	=	false;
	m_bAudioPlayShoot	=	false;
	m_bAudioPlayWalk	=	false;
	m_bAudioPlayFly		=	false;
	m_fRunTimer			= 0.0f;

	m_bBossAudioPlayCannon		=	false;
	m_bBossAudioPlayCharge2		=	false;
	m_bBossAudioPlayCharge1		=	false;
	m_bBossAudioPlayHit			=	false;
	m_bBossAudioPlayLaser2		=	false;
	m_bBossAudioPlayDebirs		=	false;
	m_bBossAudioPlayRocket		=	false;
	m_bBossAudioPlayLaser1		=	false;
	m_bBossAudioPlayStomp		=	false;
	m_bIsBossAudioPlaySpawn		=	false;
	m_bBossAudioPlayPhaseOver	=	false;

	m_bHit = false;

	//nAudioID = 210;
	//CWwiseManager::GetInstance()->AddID(nAudioID);

	CWwiseManager::GetInstance()->RegisterObject((AkGameObjectID)(ID), "Enemy");
	nAudioID = ID;
	if(this->GetType() == OBJ_HELIOS)
	{
		CWwiseManager::GetInstance()->SetAttenuation((AkGameObjectID)(ID), 0.5f);
	}
	ID++;
	//CWwiseManager::GetInstance()->SetAttenuation(ENEMY, 60);

	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;

	//Special K hates democracy
}

CEnemy::~CEnemy()
{
	//CWwiseManager::GetInstance()->UnregisterObject(ENEMY);
}

void CEnemy::Shutdown()
{
	CBase::Shutdown();
	//CWwiseManager::GetInstance()->RemoveID(nAudioID);
	//CWwiseManager::GetInstance()->UnregisterObject((AkGameObjectID)(nAudioID));
	m_bHit = false;
	m_bJumpPadTime = false;
	//nAudioID = 210;
	m_bArriving = false;
	SetCurrentTarget(NULL);
	CFXManager::GetInstance()->UnloadEffect(m_nShadowEffect);
}

void CEnemy::Input()
{
}

void CEnemy::Update(float fElapsedTime)
{
	m_fRunTimer += fElapsedTime;
	if(GetIsOnGround())
	{
		CFXManager::GetInstance()->GetEffect(m_nShadowEffect)->SetRenderType(RT_TRANSPARENT);
		D3DXVECTOR3 worldUp(0,1,0);
		float rot = acos(D3DXVec3Dot(&worldUp, &m_faceNormal));
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &worldUp, &m_faceNormal);

		D3DXMATRIX rotMat;
		D3DXMatrixRotationAxis(&rotMat, &cross, rot);

		D3DXMATRIX newMat;
		D3DXMatrixTranslation(&newMat, GetFrame().GetWorldMatrix()._41, GetFrame().GetWorldMatrix()._42 + 1.0f,
			GetFrame().GetWorldMatrix()._43);

		CFXManager::GetInstance()->GetEffect(m_nShadowEffect)->GetFrame().GetLocalMatrix() =
			rotMat * newMat ;
	}
	else
	{
		CFXManager::GetInstance()->GetEffect(m_nShadowEffect)->SetRenderType(RT_DONOTRENDER);
	}

	FindNearestEnemy();
	std::list<CBase*> test = CObjectManager::GetInstance()->GetEnemyList();
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition((AkGameObjectID)(nAudioID), pos);

	static bool bPrevSpace = false;
	static bool bCurrSpace = false;
	bPrevSpace = bCurrSpace;
	bCurrSpace = m_bIsOnGround;
	if(bPrevSpace == false && bCurrSpace == true)
	{
		m_bAudioPlayLand = true;
	}

	CBase::Update(fElapsedTime);

	if(m_bAudioPlayJump == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_JUMP, (AkGameObjectID)(nAudioID));
		m_bAudioPlayJump = false;
	}

	if(m_bAudioPlayLand == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_NOISE, (AkGameObjectID)(nAudioID));
		m_bAudioPlayLand = false;
	}

	if(m_bAudioPlayDie == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_DEATH, (AkGameObjectID)(nAudioID));
		m_bAudioPlayDie = false;
	}

	if(m_bAudioPlayHurt == true && GetIsDead() == false)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_HITS, (AkGameObjectID)(nAudioID));
		m_bAudioPlayHurt = false;
	}

	if(m_bAudioPlayShoot)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_GUNSHOT, (AkGameObjectID)(nAudioID));
		m_bAudioPlayShoot = false;
	}

	if(m_bAudioPlayWalk)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_NOISE, (AkGameObjectID)(nAudioID));
		m_bAudioPlayWalk = false;
	}

	if(m_bAudioPlayFly)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_WEAPONWHOOSH, (AkGameObjectID)(nAudioID));
		//m_bAudioPlayWalk = false;
	}

	if(m_bBossAudioPlayCannon == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_CANNONSHOT, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayCannon = false;
	}

	if(m_bBossAudioPlayCharge2 == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_CHARGE_LARGELASER, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayCharge2 = false;
	}

	if(m_bBossAudioPlayCharge1 == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_CHARGE_SMALLLASER, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayCharge1 = false;
	}

	if(m_bBossAudioPlayHit == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_HITS, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayHit = false;
	}

	if(m_bBossAudioPlayLaser2 == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_LARGELASER, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayLaser2 = false;
	}

	if(m_bBossAudioPlayDebirs == true)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_METAL_DEBRIS, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayDebirs = false;
	}

	if(m_bBossAudioPlayRocket == true)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_ROCKETLAUNCHER, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayRocket = false;
	}

	if(m_bBossAudioPlayLaser1 == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_SMALLLASER, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayLaser1 = false;
	}

	if(m_bBossAudioPlayStomp == true)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_STOMP, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayStomp = false;
	}

	if(m_bBossAudioPlayPhaseOver == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_DEATH, (AkGameObjectID)(nAudioID));
		m_bBossAudioPlayPhaseOver = false;
	}

	//if(m_bBossAudioPlayStomp)
	//{
	//	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_STOMP, ENEMY);
	//	m_bBossAudioPlayStomp = false;
	//}

	//if(m_bBossAudioPlayPhaseOver)
	//{
	//	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_EXPLOSION, ENEMY);
	//	m_bBossAudioPlayPhaseOver = false;
	//}

	//if(m_bBossAudioPlayLaser)
	//{
	//	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_LASER, ENEMY);
	//	m_bBossAudioPlayLaser = false;
	//}

	//if(m_bBossAudioPlayTarget)
	//{
	//	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_2D_UNDERATTACK, ENEMY);
	//	m_bBossAudioPlayTarget = false;
	//}

	//if(m_bBossAudioPlayMissile)
	//{
	//	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_ROCKETLAUNCHER, ENEMY);
	//	m_bBossAudioPlayMissile = false;
	//}

	if(m_fHitTimer > 0.0)
	{
		m_fHitTimer -= fElapsedTime;
		if(m_fHitTimer <= 0.0)
		{
			//CParticleManager::GetInstance()->DestroyParticle(m_nHit);
			m_bHit = false;
		}
	}
}

void CEnemy::Render()
{
}

void CEnemy::Init(D3DXVECTOR3 vPosition)
{
	SetVelocity(0.0f, 0.0f, 0.0f);
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_nShadowEffect = CFXManager::GetInstance()->LoadEffect(FX_SHADOW_BLOB, matIdentity, -1.0, true);
	m_bArriving = false;
	m_fRandVelocity = RandomFloat(25.0f,35.0f);
	SetPosition(vPosition);
}
void CEnemy::AttackTarget()
{
}

void CEnemy::MoveTowardsTarget()
{
	if(GetCurrentTarget()->GetPosition().x <= GetPosition().x)
	{
		SetVelocity(D3DXVECTOR3(-m_fRandVelocity,GetVelocity().y,GetVelocity().z));
		SetRotationY(D3DX_PI*0.5);
		SetIsFacingLeft(true);
	}
	else if(GetCurrentTarget()->GetPosition().x >= GetPosition().x)
	{
		SetVelocity(D3DXVECTOR3(m_fRandVelocity,GetVelocity().y,GetVelocity().z));
		SetRotationY(-D3DX_PI*0.5);
		SetIsFacingLeft(false);
	}
}

void CEnemy::CollisionReact(CBase *pBase)
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
			robotCenter._42 += 15.0f;
			//m_nHit = CParticleManager::GetInstance()->SpawnParticle("robothit",robotCenter);
			CParticleManager::GetInstance()->SpawnParticle("Robot_spark2", robotCenter);

			if(!m_bHit)
			{
			}
			m_fHitTimer = 0.50f;
			m_bHit = true;

			m_bAudioPlayHurt = true;
			break;
		}
	}

	switch(pBase->GetType())
	{
	case OBJ_MACHINE_GUN_BULLET:
		{
			pBase->SetIsActive(false);
			SetCurrentHealth(GetCurrentHealth() - ((CMachineGunBullet*)(pBase))->GetDamage());

			if(GetVelocity().x > 0)
			{
				SetVelocity(GetVelocity().x - 8,GetVelocity().y,GetVelocity().z);
			}
			else
			{
				SetVelocity(GetVelocity().x + 8,GetVelocity().y,GetVelocity().z);
			}

			if(CGameState::GetInstance()->GetEnemiesAlive() > 0)
			{
				CGameState::GetInstance()->SetEnemiesAlive(CGameState::GetInstance()->GetEnemiesAlive() - 1);
			}
			break;
		}
	case OBJ_SHOTGUN_BULLET:
		{
			pBase->SetIsActive(false);
			SetCurrentHealth(GetCurrentHealth() - ((CShotgunBullet*)(pBase))->GetDamage());
			break;
		}
	case OBJ_SHOTGUN_ALTERNATE:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CShotgunAlternate*)(pBase))->GetDamage());
			((CShotgunAlternate*)(pBase))->SetBore(true);
			break;
		}
	case OBJ_SHOTGUN_ULTIMATE:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CShotgunUltimate*)(pBase))->GetDamage());
			break;
		}
	case OBJ_FLAME:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CFlamethrowerBullet*)(pBase))->GetDamage());
			break;
		}
	case OBJ_NAPALM:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CFlamethrowerAlternate*)(pBase))->GetDamage());
			break;
		}
	case OBJ_FLAME_ULTIMATE:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CFlamethrowerUltimate*)(pBase))->GetDamage());
			break;
		}
	case OBJ_ROCKET_BULLET:
		{
			pBase->SetIsActive(false);
			SetCurrentHealth(GetCurrentHealth() - ((CRocketBullet*)(pBase))->GetDamage());
			break;
		}
	case OBJ_ROCKET_ALTERNATE:
		{
			if(((CRocketAlternate*)pBase)->GetOwner()->GetType() == OBJ_ULTIMUS)
				break;
			SetCurrentHealth(GetCurrentHealth() - ((CRocketAlternate*)(pBase))->GetDamage());
			break;
		}
	case OBJ_ROCKET_ULTIMATE:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CRocketUltimate*)(pBase))->GetDamage());
			break;
		}
	case OBJ_LIGHTNING:
		{
			if(((CLightningBullet*)(pBase))->GetHitTarget())
				break;

			if(m_bThunderStruck)
				break;

			SetCurrentHealth(GetCurrentHealth() - ((CLightningBullet*)(pBase))->GetDamage());
			((CLightningBullet*)(pBase))->SetHitTarget(true);
			((CLightningBullet*)(pBase))->SetCurrentEnemy(this);
			SetIsThunderStruck(true);
			break;
		}
	case OBJ_LIGHTNING_ALTERNATE:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CLightningAlternate*)(pBase))->GetDamage());

			if(((CLightningAlternate*)(pBase))->GetCurrentEnemy() == this)
				break;

			((CLightningAlternate*)(pBase))->SetIsSplitting(true);
			((CLightningAlternate*)(pBase))->SetCurrentEnemy(this);

			break;
		}
	case OBJ_LIGHTNING_ULTIMATE:
		{
			SetCurrentHealth(GetCurrentHealth() - ((CRocketUltimate*)(pBase))->GetDamage());
			break;
		}
	case OBJ_GROUND:
		{
			//Ground collision stuff
			break;
		}
	case OBJ_JUMPPAD:
		SetJumpPadTime(true);
		break;
	case OBJ_EFFECT:
		{
			CEffect* pFX = (CEffect*)pBase;
			if(pFX->GetFXType() == FX_EXPLOSION_BARREL)
			{
				SetCurrentHealth(0);
				m_bAudioPlayHurt = true;
			}
		}
	}
}

void CEnemy::SceneCollisionReact(CBase* pBase)
{
	switch(pBase->GetType())
	{
	case OBJ_PLATFORM:
		SetIsonPlatform(true);
		break;
	default:
		SetIsonPlatform(false);
		break;
	}
}

void CEnemy::FindNearestEnemy(void)
{
}

void CEnemy::PlaySpawnLoop(void)
{
	SetIsPlayBossSpawn(true);
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_SPAWN_LOOPS, (AkGameObjectID)(nAudioID));
}

void CEnemy::StopSpawnLoop(void)
{
	SetIsPlayBossSpawn(false);
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_BOSS_SPAWN_LOOPS, (AkGameObjectID)(nAudioID));
}