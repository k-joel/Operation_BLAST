#include "CRocketBullet.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Camera/Camera.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../../LuaEngine/CLuaEngine.h"


CRocketBullet::CRocketBullet()
{
	SetType(OBJ_ROCKET_BULLET);
	despawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetIsAffectedByGrav(false);
	m_bCreated = true;
}

CRocketBullet::CRocketBullet(const CRocketBullet& blt)
{
}

CRocketBullet::~CRocketBullet()
{
}

void CRocketBullet::Update(float fDt)
{
		CBase::Update(fDt);

		despawnTimer += fDt;

		if(despawnTimer > m_fMaxLife)
		{
			SetIsActive(false);
			despawnTimer = 0.0f;
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_HEALTH_LOSS, 200);
		}

		if(GetIsOnGround())
		{
			SetIsActive(false);
			despawnTimer = 0.0f;
			CParticleManager::GetInstance()->SpawnParticle("Explosion2", GetFrame().GetWorldMatrix());
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER_EXPLO, 200);
		}
}

void CRocketBullet::Render()
{
	CBase::Render();
}

bool CRocketBullet::CheckCollision(CBase* pBase)
{
	
	return CBase::CheckCollision(pBase);
}

void CRocketBullet::CollisionReact(CBase* pBase)
{
	//The bullet will only react to hitting the ground

	switch(pBase->GetType())
	{
	case OBJ_GROUND:
		{
			SetIsActive(false);
			//Play a sound here?  WHO KNOWS!  =D
			break;
		}
	}
}
void CRocketBullet::Shutdown()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRocketAlternate::CRocketAlternate()
{
	SetType(OBJ_ROCKET_ALTERNATE);
	m_fDespawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetIsAffectedByGrav(false);
	m_bCreated = true;
}

CRocketAlternate::~CRocketAlternate()
{
}

void CRocketAlternate::Render()
{
	CBase::Render();
}

void CRocketAlternate::Update(float fDt)
{
	if(fDt <= 0.0f)
		return;

	CBase::Update(fDt);
	SetPosition(GetPosition().x, (sin(GetPosition().x* 0.05f)) + GetPosition().y, GetPosition().z);

	m_fDespawnTimer += fDt;

	//correct direction and forward
	D3DXVECTOR3 nDir, curX, newY;
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
	nDir = m_vPosition-m_vPrevPosition;
	D3DXVec3Normalize(&nDir, &nDir);

	D3DXVec3Cross(&newY, &nDir, &curX);
	D3DXVec3Normalize(&newY, &newY);

	newOrien._21 = newY.x;
	newOrien._22 = newY.y;
	newOrien._23 = newY.z;

	newOrien._31 = nDir.x;
	newOrien._32 = nDir.y;
	newOrien._33 = nDir.z;

	GetFrame().GetLocalMatrix() = newOrien;

	if(m_fDespawnTimer > 1.5f)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS:, 200);
	}

	if(GetIsOnGround())
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
		CParticleManager::GetInstance()->SpawnParticle("Explosion2", GetFrame().GetWorldMatrix());
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_HEALTH_LOSS, 200);
	}
}

bool CRocketAlternate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CRocketAlternate::CollisionReact(CBase* pBase)
{

}

void CRocketAlternate::Shutdown()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRocketUltimate::CRocketUltimate()
{
	SetType(OBJ_ROCKET_ULTIMATE);
	m_fDespawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetIsAffectedByGrav(true);
}

CRocketUltimate::~CRocketUltimate()
{
}

void CRocketUltimate::Render()
{
	CBase::Render();
}

void CRocketUltimate::Update(float fDt)
{
	CBase::Update(fDt);

	m_fDespawnTimer += fDt;

	if(m_fDespawnTimer > 5.0f)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_HEALTH_LOSS, 200);
	}

	if(GetIsOnGround())
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_HEALTH_LOSS, 200);
	}
}

bool CRocketUltimate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CRocketUltimate::CollisionReact(CBase* pBase)
{

}

void CRocketUltimate::Shutdown()
{
}