#include "CShotgunBullet.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Camera/Camera.h"
#include "../../Rendering/CRenderContext.h"

#include "../../FX/CFXManager.h"
#include "../../Game Objects/Weapon States/CShotgunState.h"
#include "../../Game Objects/CObjectFactory.h"
#include "../../Game Objects/CObjectManager.h"

//const AkGameObjectID SHOTGUN_ENVIRONMENT_DESTROY = 100;
const AkGameObjectID Ak_SHOTGUN_B = 12;

#define MAX_BOUNCES 0

CShotgunBullet::CShotgunBullet()
{
	SetType(OBJ_SHOTGUN_BULLET);
	m_fDespawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetIsAffectedByGrav(false);
	m_pBV = &m_Sphere;

	CWwiseManager::GetInstance()->RegisterObject(Ak_SHOTGUN_B, "Shotgun Bullet");
}

CShotgunBullet::~CShotgunBullet()
{
}

void CShotgunBullet::Render()
{
	CBase::Render();
}

void CShotgunBullet::Update(float fDt)
{
	if(m_pOwner)
	{
		SetPosition(GetPosition().x, GetPosition().y, (GetPosition().z+m_pOwner->GetPosition().z)/2.0f);
	}

	//GetAnimCtrl().update(fDt);
	CBase::Update(fDt);
	static AkSoundPosition pos;
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(Ak_SHOTGUN_B, pos);

	m_fDespawnTimer += fDt;

	if(m_fDespawnTimer > m_fMaxLife)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}

	if(GetIsOnGround()||GetHitWall())
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BULLET_HIT_GROUND, Ak_SHOTGUN_B);
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}
}

bool CShotgunBullet::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CShotgunBullet::CollisionReact(CBase* pBase)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CShotgunAlternate::CShotgunAlternate()
{
	SetType(OBJ_SHOTGUN_ALTERNATE);
	m_fDespawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetDamage(10.0f);
	SetIsAffectedByGrav(false);
	m_pBV = &m_Sphere;

	//CWwiseManager::GetInstance()->RegisterObject(Ak_SHOTGUN_B, "Shotgun Env Collide");
}

CShotgunAlternate::~CShotgunAlternate()
{
}

void CShotgunAlternate::Render()
{
	CBase::Render();
}

void CShotgunAlternate::Update(float fDt)
{
	//GetAnimCtrl().update(fDt);
	CBase::Update(fDt);
	static AkSoundPosition pos;
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(Ak_SHOTGUN_B, pos);

	D3DXVECTOR3 tmpPos = GetPosition();
	tmpPos.z = GetOwner()->GetPosition().z;
	SetPosition(tmpPos);

	D3DXVECTOR3 nDir, curX, newY;
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
	nDir = -GetVelocity();
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

	if(m_bBore)
	{
		m_fBoreTimer -= fDt;

		D3DXMATRIX wobble;
		D3DXMatrixRotationX(&wobble, (rand()%2)?(D3DX_PI/20.0f):(-D3DX_PI/20.0f));

		GetFrame().GetLocalMatrix() = wobble*GetFrame().GetLocalMatrix();

		if(m_fBoreTimer <= 0.0f)
		{
			m_vVelocity = m_vNormalVel;
			m_bBore = false;
		}
	}

	if(m_bBore && !m_bPrevBore)
	{
		m_pBoreEffect->SetIsActive(false);
		m_pBoreEffect = CParticleManager::GetInstance()->SpawnParticle("Shotgun_altB", GetFrame().GetWorldMatrix());
		m_pBoreEffect->SetRotation(m_fRotation);
	}
	else if(!m_bBore && m_bPrevBore)
	{
		m_pBoreEffect->SetIsActive(false);
		m_pBoreEffect = CParticleManager::GetInstance()->SpawnParticle("Shotgun_altA", GetFrame().GetWorldMatrix());
		m_pBoreEffect->SetRotation(m_fRotation);
	}

	m_pBoreEffect->SetWorld(GetFrame().GetWorldMatrix());

	m_fDespawnTimer += fDt;

	if(m_fDespawnTimer > m_fMaxLife)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
		m_pBoreEffect->SetIsActive(false);
	}

	if(GetIsOnGround()||GetHitWall())
	{
		SetIsActive(false);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BULLET_HIT_GROUND, Ak_SHOTGUN_B);
		m_fDespawnTimer = 0.0f;
		m_pBoreEffect->SetIsActive(false);
	}

	//CFXManager::GetInstance()->LoadEffect(FX_SG_ALT_TRAIL, GetFrame().GetWorldMatrix(), 0.1f, false);

	m_bPrevBore = m_bBore;
}
void CShotgunAlternate::Shutdown()
{
	m_pBoreEffect->SetIsActive(false);
}

bool CShotgunAlternate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CShotgunAlternate::CollisionReact(CBase* pBase)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CShotgunUltimate::CShotgunUltimate()
{
	SetType(OBJ_SHOTGUN_ULTIMATE);
	m_nBounceCount = 0;
	SetIsAffectedByGrav(false);
	m_pBV = &m_Sphere;

	luaState = lua_open();
	luaL_openlibs(luaState);
	SetDamage(5.0f);
	luaL_dofile(luaState, "Resources/Lua/Weapons/Shotgun.lua");

	//CWwiseManager::GetInstance()->RegisterObject(SHOTGUN_ENVIRONMENT_DESTROY, "Shotgun Env Collide");
}

CShotgunUltimate::~CShotgunUltimate()
{
	lua_close(luaState);
}

void CShotgunUltimate::Initialize()
{
	m_fSpawnMore = 0.0f;
	CBase::Initialize();
	m_pEffect = NULL;
}

void CShotgunUltimate::Render()
{
	CBase::Render();
}

void CShotgunUltimate::Update(float fDt)
{
	//GetAnimCtrl().update(fDt);
	CBase::Update(fDt);
	static AkSoundPosition pos;
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(Ak_SHOTGUN_B, pos);

	//correct direction and forward
	D3DXVECTOR3 nDir, curX, newY;
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
	nDir = -GetVelocity();
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

	m_fSpawnMore += fDt;

	if(CCollisionTests::IntersectFrustrumSphere(m_Sphere.GetSphere(), CCamera::GetInstance()->GetFrustum()) == CT_INTERSECT)
	{
		D3DXVECTOR3 vel = GetVelocity();
		ChangeBounce(vel);
		SetVelocity(vel);
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BULLET_RICOCHET, Ak_SHOTGUN_B);
	}

	if(GetIsOnGround()||GetHitWall())
	{
		D3DXVECTOR3 vel = m_previousVel;
		ChangeBounce(vel);
		SetVelocity(vel);
		SetPosition(GetPosition().x, GetPosition().y + 1.0f, GetPosition().z);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BULLET_HIT_GROUND, Ak_SHOTGUN_B);
	}
	else
	{
		m_previousVel = GetVelocity();
	}

	//Killing the bullet on the fourth bounce.
	if(m_nBounceCount > MAX_BOUNCES)
	{
		m_nBounceCount = 0;
		SetIsActive(false);
	}

	//CFXManager::GetInstance()->LoadEffect(FX_SG_ALT_TRAIL, GetFrame().GetWorldMatrix(), 0.1f, false);
}

bool CShotgunUltimate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CShotgunUltimate::CollisionReact(CBase* pBase)
{
	//if we hit anything spawn more
	switch(pBase->GetType())
	{
	case OBJ_ASSAULTER:
	case OBJ_STRIKER:
	case OBJ_HELIOS:
	case OBJ_CRATE:
		if(!((CEnemy*)pBase)->GetIsHitByShotgun())
		{
			((CEnemy*)pBase)->SetIsHitByShotgunUlt(true);
			m_fSpawnMore = 0.0f;

			CShotgunUltimate* bullets[7];
			int x = 0;
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			for(; x < 7; ++x)
			{
				bullets[x] = (CShotgunUltimate*)(CObjectFactory::GetInstance()->GetObject<CShotgunUltimate>(OBJ_SHOTGUN_ULTIMATE));
				bullets[x]->SetIsActive(true);
				bullets[x]->SetOwner(m_pOwner);
				bullets[x]->SetPosition(GetPosition());
				bullets[x]->SetEffect(CParticleManager::GetInstance()->SpawnParticle("Shotgun_ult", iden, (D3DXMATRIX*)&bullets[x]->GetFrame().GetWorldMatrix()));
			}

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, 0);
			lua_pushboolean(luaState, true);
			lua_pushnumber(luaState, ULTIMATE_FIRE);

			lua_pcall(luaState, 3, 29, 0);

			bullets[0]->SetVelocity((float)lua_tonumber(luaState, -28), (float)lua_tonumber(luaState, -27), 0.0f);
			bullets[0]->SetRotationY((float)lua_tonumber(luaState, -26));
			bullets[0]->SetRotationZ((float)lua_tonumber(luaState, -25));

			bullets[1]->SetVelocity((float)lua_tonumber(luaState, -24), (float)lua_tonumber(luaState, -23), 0.0f);
			bullets[1]->SetRotationY((float)lua_tonumber(luaState, -22));
			bullets[1]->SetRotationZ((float)lua_tonumber(luaState, -21));

			bullets[2]->SetVelocity((float)lua_tonumber(luaState, -20), (float)lua_tonumber(luaState, -19), 0.0f);
			bullets[2]->SetRotationY((float)lua_tonumber(luaState, -18));
			bullets[2]->SetRotationZ((float)lua_tonumber(luaState, -17));

			bullets[3]->SetVelocity((float)lua_tonumber(luaState, -16), (float)lua_tonumber(luaState, -15), 0.0f);
			bullets[3]->SetRotationY((float)lua_tonumber(luaState, -14));
			bullets[3]->SetRotationZ((float)lua_tonumber(luaState, -13));

			bullets[4]->SetVelocity((float)lua_tonumber(luaState, -12), (float)lua_tonumber(luaState, -11), 0.0f);
			bullets[4]->SetRotationY((float)lua_tonumber(luaState, -10));
			bullets[4]->SetRotationZ((float)lua_tonumber(luaState, -9));

			bullets[5]->SetVelocity((float)lua_tonumber(luaState, -8), (float)lua_tonumber(luaState, -7), 0.0f);
			bullets[5]->SetRotationY((float)lua_tonumber(luaState, -6));
			bullets[5]->SetRotationZ((float)lua_tonumber(luaState, -5));

			bullets[6]->SetVelocity((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f);
			bullets[6]->SetRotationY((float)lua_tonumber(luaState, -2));
			bullets[6]->SetRotationZ((float)lua_tonumber(luaState, -1));

			for(x = 0; x < 7; ++x)
			{
				bullets[x]->SetDamage((float)lua_tonumber(luaState, -29));
			}
			lua_pop(luaState, 29);

			for(x = 0; x < 7; ++x)
			{
				CObjectManager::GetInstance()->AddObject(bullets[x]);
			}
			//SetIsActive(false);
		}
		break;
	}
}

void CShotgunUltimate::ChangeBounce(D3DXVECTOR3& currentVel)
{
	//Until the frustum collision is in, there's no good way to tell where I hit and what way to bounce.
	//So for now, just reversing the direction.
	//currentVel.x = -currentVel.x;
	//currentVel.y = -currentVel.y;

	bool bounced = false;
	D3DXVECTOR3 dirVec = CCamera::GetInstance()->GetViewPosition() - GetPosition();

	if((currentVel.x < 0.0f && dirVec.x > 0.0f) ||
		(currentVel.x > 0.0f && dirVec.x < 0.0f))
	{
		bounced = true;
		currentVel.x = -currentVel.x;
	}

	if((currentVel.y < 0.0f && dirVec.y > 0.0f) ||
		(currentVel.y > 0.0f && dirVec.y < 0.0f))
	{
		bounced = true;
		currentVel.y = -currentVel.y;
	}

	if(bounced == true)
	{
		++m_nBounceCount;
	}
}

void CShotgunUltimate::Shutdown()
{
	if(m_pEffect)
		m_pEffect->SetIsActive(false);
}