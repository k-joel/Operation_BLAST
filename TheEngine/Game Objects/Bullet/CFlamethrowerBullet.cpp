#include "CFlamethrowerBullet.h"
#include "../Weapon States/CFlamethrowerState.h"
#include "../../Mesh/CModel.h"
#include "../../States/CLoadState.h"
#include "../../Mesh/CModelManager.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Rendering/CRenderContextManager.h"
#include "../../Camera/Camera.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"
#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

#include "../../fx/CParticleManager.h"

#include "../../Spacial Tree/CHGrid.h"

#include "../../fx/Helpers.h"

#define MAX_BOUNCES 2

const AkGameObjectID AK_FLAME_ALT = 150;
const AkGameObjectID AK_FLAME_ULT_0 = 151;
const AkGameObjectID AK_FLAME_ULT_1 = 152;

CFlamethrowerBullet::CFlamethrowerBullet()
{
	SetType(OBJ_FLAME);
	SetRenderType(RT_DONOTRENDER);
	SetIsAffectedByGrav(false);
	SetDamage(0.5f);

	D3DXMatrixIdentity(&m_mxFakeBoneMat);
	m_pBV = &m_Sphere;
}

void CFlamethrowerBullet::Initialize()
{
	CBase::Initialize();
	SetType(OBJ_FLAME);
	SetDamage(0.0f);
	m_fDespawnTimer = 0.0f;

	m_Sphere.GetSphere().r = 10.0f;
	m_Sphere.GetSphere().c = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Sphere.GetOriginalSphere() = m_Sphere.GetSphere();

	m_AABB.GetAABB().minPt = D3DXVECTOR3(-10, -10, -10);
	m_AABB.GetAABB().maxPt = D3DXVECTOR3(10, 10, 10);
	m_AABB.GetAABB().center = D3DXVECTOR3(0, 0, 0);
	m_AABB.GetOriginalAABB() = m_AABB.GetAABB();
}

CFlamethrowerBullet::~CFlamethrowerBullet()
{
}

void CFlamethrowerBullet::Shutdown()
{
	m_pEffect->SetIsActive(false);
	//CParticleManager::GetInstance()->DestroyParticle(m_fxID);
}

void CFlamethrowerBullet::Update(float fDt)
{
	if(m_pOwner)
	{
		SetPosition(GetPosition().x, GetPosition().y, (GetPosition().z+m_pOwner->GetPosition().z)/2.0f);
	}

	m_fDespawnTimer += fDt;

	if(m_fDespawnTimer >= m_fMaxLife)
	{
		//if(GetOwner()->GetType() == OBJ_BARRON)
		//{
		//	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_LOOP, 4); // Playing the sound.  Pew pew!
		//	//CFlamethrowerState::GetInstance()->SetIsPlayFlameNorm(false);
		//	((CPlayer*)(GetOwner()))->SetAudioFlameNorm(false);
		//}
		//else if(GetOwner()->GetType() == OBJ_CLAIRE)
		//{
		//	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_LOOP, 5); // Playing the sound.  Pew pew!
		//	//CFlamethrowerState::GetInstance()->SetIsPlayFlameNorm(false);
		//	((CPlayer*)(GetOwner()))->SetAudioFlameNorm(false);
		//}
		SetIsActive(false);
	}

	D3DXMATRIX gunMat = m_pOwner->GetFrameAt(1).GetWorldMatrix();
	D3DXVECTOR3 hardCodedGunTip(0.0f, 5.0f, -17.0f);

	D3DXMATRIX offset, final;
	D3DXMatrixTranslation(&offset, hardCodedGunTip.x, hardCodedGunTip.y, hardCodedGunTip.z);

	m_mxFakeBoneMat = offset * gunMat;

	CBase::Update(fDt);

	m_pEffect->SetAttached(&m_mxFakeBoneMat);

	//m_AABB.CreateMesh(&m_AABB);
	//m_BVShape.SetMesh(m_AABB.GetMesh());
	//m_BVShape.GetRenderContext()->AddRenderNode(&m_BVShape);

	//m_Sphere.GetSphere() = m_pEffect->GetEmitterList()[0]->GetSphere().GetSphere();

	// update hgrid
	//CHGrid::GetInstance()->RemoveObjectFromHGrid(this);
	//CHGrid::GetInstance()->AddObjectToHGrid(this);
}

void CFlamethrowerBullet::Render()
{
	CBase::Render();
}

bool CFlamethrowerBullet::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
	//return m_pEffect->CheckCollision(pBase->GetBV());
}

void CFlamethrowerBullet::CollisionReact(CBase *pBase)
{
}

//--------------------------------------------------------------------
//
//Alternate
//
//--------------------------------------------------------------------

CFlamethrowerAlternate::CFlamethrowerAlternate()
{
	SetType(OBJ_FLAME_ALTERNATE);
	SetIsAffectedByGrav(false);
	m_fDespawnTimer = 0.0f;
	CWwiseManager::GetInstance()->RegisterObject(AK_FLAME_ALT, "Flame Alt Bullet");
	m_pBV = &m_Sphere;
}

void CFlamethrowerAlternate::Initialize()
{
	SetType(OBJ_FLAME_ALTERNATE);
	SetDamage(0.0f);
	SetIsAffectedByGrav(true);
	m_fDespawnTimer = 0.0f;
	CWwiseManager::GetInstance()->RegisterObject(AK_FLAME_ALT, "Flame Alt Bullet");
}

CFlamethrowerAlternate::~CFlamethrowerAlternate()
{
}

void CFlamethrowerAlternate::Update(float fDt)
{
	CBase::Update(fDt);
	static AkSoundPosition pos;
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(AK_FLAME_ALT, pos);

	//correct direction and forward
	D3DXVECTOR3 nDir, curX, newY;
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
	nDir = GetVelocity();
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

	m_fDespawnTimer += fDt;

	if(m_fDespawnTimer > m_fMaxLife)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;

		/*if(GetOwner()->GetType() == OBJ_BARRON)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_FLAMETHROWER_ALT, 150);
		}
		else if(GetOwner()->GetType() == OBJ_CLAIRE)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_FLAMETHROWER_ALT, 150);
		}*/
	}

	if(GetIsOnGround())
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;

		CNapalm* puddle = (CNapalm*)CObjectFactory::GetInstance()->GetObject<CNapalm>(OBJ_NAPALM);
		puddle->SetPosition(GetPosition());
		puddle->SetDamage(m_fNapalmDamage);
		puddle->SetMaxLife(m_fNapalmDuration);
		puddle->SetIsActive(true);
		puddle->SetSpawnTime(m_fNapalmFrequency);
		puddle->SetOwner(GetOwner());

		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, GetPosition().x, GetPosition().y, GetPosition().z);
		puddle->SetEffect(CParticleManager::GetInstance()->SpawnParticle("Flamethrower_alt", mat));

		CObjectManager::GetInstance()->AddObject(puddle);
	}
}

void CFlamethrowerAlternate::Render()
{
	CBase::Render();
}

bool CFlamethrowerAlternate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CFlamethrowerAlternate::CollisionReact(CBase* pBase)
{
}

//--------------------------------------------------------------------
//
//Ultimate
//
//--------------------------------------------------------------------

CFlamethrowerUltimate::CFlamethrowerUltimate()
{
	SetType(OBJ_FLAME_ULTIMATE);
	m_nBounceCount = 0;
	SetIsAffectedByGrav(false);
	CWwiseManager::GetInstance()->RegisterObject(AK_FLAME_ULT_0, "Flame Ultimate 0");
	CWwiseManager::GetInstance()->RegisterObject(AK_FLAME_ULT_1, "Flame Ultimate 1");
	m_pBV = &m_Sphere;
	m_pEffect = NULL;
	m_fLifeTime = 0.0f;
}

CFlamethrowerUltimate::~CFlamethrowerUltimate()
{
	if(m_pEffect)
	{
		m_pEffect->SetIsActive(false);
		m_pEffect = NULL;
	}
}

void CFlamethrowerUltimate::Render()
{
	CBase::Render();
}

void CFlamethrowerUltimate::Update(float fDt)
{
	CBase::Update(fDt);
	static AkSoundPosition pos;
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition((AkGameObjectID)(nAudioID), pos);

	if(CCollisionTests::IntersectFrustrumSphere(m_Sphere.GetSphere(), CCamera::GetInstance()->GetFrustum()) == CT_OUT)
	{
		D3DXVECTOR3 vel = GetVelocity();
		ChangeBounce(vel);
		SetVelocity(vel);
	}

	//Killing the bullet on the fourth bounce.
	if(m_nBounceCount >= MAX_BOUNCES)
	{
		m_nBounceCount = 0;
		SetIsActive(false);
		if(m_pOwner->GetType() == OBJ_BARRON)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_ULTIMATE_SHOT, (AkGameObjectID)(nAudioID));
		}
		else if(m_pOwner->GetType() == OBJ_CLAIRE)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_ULTIMATE_SHOT, (AkGameObjectID)(nAudioID));
		}
	}

	//spin & flames
	m_fLifeTime += fDt;

	D3DXMATRIX spin;
	D3DXMatrixRotationY(&spin, D3DX_PI*3.0f*m_fLifeTime);

	GetFrame().GetLocalMatrix() = spin * GetFrame().GetLocalMatrix();

	if(m_fLifeTime >= 0.1f)
	{
		m_fLifeTime = 0.0f;
		CParticleManager::GetInstance()->SpawnParticle("Explosion2", GetFrame().GetWorldMatrix());
	}
}

bool CFlamethrowerUltimate::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CFlamethrowerUltimate::CollisionReact(CBase* pBase)
{
}

void CFlamethrowerUltimate::ChangeBounce(D3DXVECTOR3& currentVel)
{
	bool bounced = false;
	D3DXVECTOR3 dirVec = CCamera::GetInstance()->GetViewPosition() - GetPosition();

	if((currentVel.x < 0.0f && dirVec.x > 0.0f) ||
		(currentVel.x > 0.0f && dirVec.x < 0.0f))
	{
		bounced = true;
		currentVel.x = -currentVel.x;
	}

	currentVel.y = 200.0f;

	if(bounced == true)
	{
		++m_nBounceCount;
	}
}

void CFlamethrowerUltimate::Shutdown()
{
	if(m_pEffect)
	{
		m_pEffect->SetIsActive(false);
		m_pEffect = NULL;
	}
}

//******************************************************************************
//Napalm puddle
//******************************************************************************
CNapalm::CNapalm()
{
}

CNapalm::CNapalm(const CNapalm& blt)
{
}

CNapalm::~CNapalm()
{
}

void CNapalm::Initialize(void)
{
	m_fLifeTimer = 0.0f;
	m_fDamage = 0.0f;
	SetType(OBJ_NAPALM);
	m_fSpawnTimer = 0.0f;
	m_pEffect = NULL;
}

void CNapalm::Update(float fDt)
{
	CBase::Update(fDt);
	m_fLifeTimer += fDt;
	m_fSpawnTimer += fDt;

	SetRenderType(RT_DONOTRENDER);
	if(m_fLifeTimer >= m_fMaxLife)
	{
		SetIsActive(false);
		if(GetOwner()->GetType() == OBJ_BARRON)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_FLAMETHROWER_ALT, 150);
		}
		else if(GetOwner()->GetType() == OBJ_CLAIRE)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_FLAMETHROWER_ALT, 150);
		}
	}
}

void CNapalm::Render(void)
{
	CBase::Render();
}

void CNapalm::Shutdown(void)
{
	if(m_pEffect)
		m_pEffect->SetIsActive(false);
}