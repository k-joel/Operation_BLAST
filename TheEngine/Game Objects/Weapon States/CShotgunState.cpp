#include "CShotgunState.h"
#include "../CBase.h"
#include "../Bullet/CShotgunBullet.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"
#include "../../LuaEngine/CLuaEngine.h"
#include "../../FX/CParticleManager.h"
#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"
#include "../../Camera/Camera.h"

//const AkGameObjectID SHOTGUN_FIRE_NORMAL	= 100;
//const AkGameObjectID SHOTGUN_FIRE_ALT		= 200;
//const AkGameObjectID SHOTGUN_FIRE_ULT		= 300;

const AkGameObjectID Ak_SHOTGUN = 11;

CShotgunState::CShotgunState()
{
	m_nWeaponType = SHOTGUN;
	m_pMesh = &CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("ShotgunShape"))->GetMesh();

	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Weapons/Shotgun.lua");

	CWwiseManager::GetInstance()->RegisterObject(SHOTGUN, "Shotgun");
}

CShotgunState::~CShotgunState()
{
	lua_close(luaState);
}

CShotgunState* CShotgunState::GetInstance(void)
{
	static CShotgunState instance;
	return &instance;
}

void CShotgunState::Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType)
{
	D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
	D3DXVECTOR3 hardCodedGunTip(0.0f, 5.0f, -25.0f);

	D3DXVECTOR4 realPos;
	D3DXVec3Transform(&realPos, &hardCodedGunTip, &gunMat);
	pos.x = realPos.x;
	pos.y = realPos.y;
	pos.z = realPos.z;

	switch(shotType)
	{
	case NORMAL_FIRE:
		{
			if(owner->GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_SHOTGUN, 4);
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_SHOTGUN, 5);
			}

			CShotgunBullet* bulletMid = (CShotgunBullet*)(CObjectFactory::GetInstance()->GetObject<CShotgunBullet>(OBJ_SHOTGUN_BULLET));
			CShotgunBullet* bulletLeft = (CShotgunBullet*)(CObjectFactory::GetInstance()->GetObject<CShotgunBullet>(OBJ_SHOTGUN_BULLET));
			CShotgunBullet* bulletRight = (CShotgunBullet*)(CObjectFactory::GetInstance()->GetObject<CShotgunBullet>(OBJ_SHOTGUN_BULLET));

			bulletMid->SetOwner(owner);
			bulletLeft->SetOwner(owner);
			bulletRight->SetOwner(owner);

			bulletMid->SetPosition(pos);
			bulletLeft->SetPosition(pos);
			bulletRight->SetPosition(pos);

			bulletMid->SetIsActive(true);
			bulletLeft->SetIsActive(true);
			bulletRight->SetIsActive(true);

			bulletMid->SetType(OBJ_SHOTGUN_BULLET);
			bulletLeft->SetType(OBJ_SHOTGUN_BULLET);
			bulletRight->SetType(OBJ_SHOTGUN_BULLET);

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3,14, 0);

			bulletMid->SetVelocity((float)lua_tonumber(luaState, -12), (float)lua_tonumber(luaState, -11), 0.0f);
			bulletLeft->SetVelocity((float)lua_tonumber(luaState, -8), (float)lua_tonumber(luaState, -7), 0.0f);
			bulletRight->SetVelocity((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f);

			bulletMid->SetRotationY((float)lua_tonumber(luaState, -10));
			bulletMid->SetRotationZ((float)lua_tonumber(luaState, -9));
			bulletMid->SetDamage((float)lua_tonumber(luaState, -13));

			bulletLeft->SetRotationY((float)lua_tonumber(luaState, -6));
			bulletLeft->SetRotationZ((float)lua_tonumber(luaState, -6));
			bulletLeft->SetDamage((float)lua_tonumber(luaState, -13));

			bulletRight->SetRotationY((float)lua_tonumber(luaState, -2));
			bulletRight->SetRotationZ((float)lua_tonumber(luaState, -2));
			bulletRight->SetDamage((float)lua_tonumber(luaState, -13));

			bulletMid->SetMaxLife((float)lua_tonumber(luaState, -14));
			bulletLeft->SetMaxLife((float)lua_tonumber(luaState, -14));
			bulletRight->SetMaxLife((float)lua_tonumber(luaState, -14));

			lua_pop(luaState, 14);

			CObjectManager::GetInstance()->AddObject(bulletMid);
			CObjectManager::GetInstance()->AddObject(bulletLeft);
			CObjectManager::GetInstance()->AddObject(bulletRight);

			//muzzle flash
			D3DXMATRIX rot, trans, muzzleMat = owner->GetFrameAt(1).GetWorldMatrix();
			D3DXMatrixTranslation(&trans, 0.0f, 5.1f, -27.0f);
			D3DXMatrixRotationY(&rot, D3DX_PI/2.0f);
			int id = CFXManager::GetInstance()->LoadEffect(FX_SG_MUZZLEFLASH, rot * trans * muzzleMat, 0.1f, false );

			break;
		}
	case ALTERNATE_FIRE:
		{
			if(owner->GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_SHOTGUN_ALT, 4);
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_SHOTGUN_ALT, 5);
			}

			CShotgunAlternate* bullet = (CShotgunAlternate*)(CObjectFactory::GetInstance()->GetObject<CShotgunAlternate>(OBJ_SHOTGUN_ALTERNATE));

			bullet->SetType(OBJ_SHOTGUN_ALTERNATE);
			bullet->SetOwner(owner);
			bullet->SetPosition(pos);
			bullet->SetIsActive(true);

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3, 6, 0);

			bullet->SetVelocity((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f);
			bullet->SetNormalVel(bullet->GetVelocity());
			bullet->SetBoreVel(bullet->GetVelocity() * 0.1f);
			bullet->SetRotationY((float)lua_tonumber(luaState, -2));
			bullet->SetRotationZ((float)lua_tonumber(luaState, -1));
			bullet->SetDamage((float)lua_tonumber(luaState, -5));

			bullet->SetMaxLife((float)lua_tonumber(luaState, -6));

			lua_pop(luaState, 6);

			CParticleEffect *effect = CParticleManager::GetInstance()->SpawnParticle("Shotgun_altA", bullet->GetFrame().GetWorldMatrix());
			bullet->SetBoreEffect(effect);//, (D3DXMATRIX*)&bullet->GetFrame().GetWorldMatrix());//, (D3DXMATRIX*)&mat);

			float rot = 0.0f;
			switch(direction)
			{
			case STRAIGHT_UP:
				rot = D3DX_PI * 0.5f;
				break;
			case ANGLE_UP:
				rot = owner->GetIsFacingLeft() ? D3DX_PI * 0.25f : D3DX_PI * 0.75f;
				break;
			case STRAIGHT_FORWARD:
				rot = owner->GetIsFacingLeft() ? D3DX_PI : 0.0f;
				break;
			case ANGLE_DOWN:
				rot = owner->GetIsFacingLeft() ? D3DX_PI * -0.25f : D3DX_PI * -0.75f;
				break;
			case STRAIGHT_DOWN:
				rot = D3DX_PI * -0.5f;
				break;
			}

			effect->SetRotation(rot);
			bullet->SetRotation(rot);

			CObjectManager::GetInstance()->AddObject(bullet);

			//muzzle flash
			D3DXMATRIX trans, muzzleMat = owner->GetFrameAt(1).GetWorldMatrix();
			D3DXMatrixTranslation(&trans, 0.0f, 5.1f, -27.0f);
			int id = CFXManager::GetInstance()->LoadEffect(FX_SG_ALT_MUZZLEFLASH, trans * muzzleMat, 0.0625, false );

			break;
		}
	}
}

void CShotgunState::FireUltimate(CBase* owner, D3DXVECTOR3 pos)
{
	if(owner->GetType() == OBJ_BARRON)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_SHOTGUN_ULTIMATE, 4);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_SHOTGUNULT, 4);
	}
	else if(owner->GetType() == OBJ_CLAIRE)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_SHOTGUN_ULTIMATE, 5);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_SHOTGUNULT, 5);
	}

	CShotgunUltimate* bullets[7];
	int x = 0;
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	for(; x < 7; ++x)
	{
		bullets[x] = (CShotgunUltimate*)(CObjectFactory::GetInstance()->GetObject<CShotgunUltimate>(OBJ_SHOTGUN_ULTIMATE));
		bullets[x]->SetIsActive(true);
		bullets[x]->SetOwner(owner);
		bullets[x]->SetPosition(pos);
		bullets[x]->m_fSpawnMore = 1.1f;
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

	CCamera::GetInstance()->ShakeCamera(1.5f);
}

int CShotgunState::GetMaxAmmo(void)
{
	lua_getglobal(luaState, "GetShotgunData");
	lua_pushstring(luaState, "MaxAmmo");
	lua_pcall(luaState, 1, 1, 0);

	int ammo = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return ammo;
}

float CShotgunState::GetFiringRate(void)
{
	lua_getglobal(luaState, "GetShotgunData");
	lua_pushstring(luaState, "NormalFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

float CShotgunState::GetAlternateRate(void)
{
	lua_getglobal(luaState, "GetShotgunData");
	lua_pushstring(luaState, "AlternateFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

int CShotgunState::GetAlternateCost(void)
{
	lua_getglobal(luaState, "GetShotgunData");
	lua_pushstring(luaState, "AltCost");
	lua_pcall(luaState, 1, 1, 0);

	int cost = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return cost;
}