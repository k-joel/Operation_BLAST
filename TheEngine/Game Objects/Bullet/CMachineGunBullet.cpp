#include "CMachineGunBullet.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Camera/Camera.h"
#include "../../Rendering/CRenderContext.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

const AkGameObjectID Ak_MGUN_B = 10;

CMachineGunBullet::CMachineGunBullet()
{
	SetType(OBJ_MACHINE_GUN_BULLET);
	m_fDespawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetIsAffectedByGrav(false);
	CWwiseManager::GetInstance()->RegisterObject(Ak_MGUN_B, "Machine Gun Bullet");
	m_pBV = &m_Sphere;
}

CMachineGunBullet::CMachineGunBullet(const CMachineGunBullet& blt)
{
}

CMachineGunBullet::~CMachineGunBullet()
{
}

void CMachineGunBullet::Update(float fDt)
{
	//GetAnimCtrl().update(fDt);
	CBase::Update(fDt);
	//SetVelocity(D3DXVECTOR3(50.0,0.0,0.0));

	m_fDespawnTimer += fDt;

	if(m_fDespawnTimer > m_fMaxLife)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}

	if(GetIsOnGround()|| GetHitWall())
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BULLET_HIT_GROUND, Ak_MGUN_B);
	}
}

void CMachineGunBullet::Render()
{
	CBase::Render();
}

bool CMachineGunBullet::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CMachineGunBullet::CollisionReact(CBase* pBase)
{
}