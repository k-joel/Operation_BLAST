#include "CFlamethrowerState.h"
#include "../CBase.h"
#include "../Bullet/CFlamethrowerBullet.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"
#include "../../Camera/Camera.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

#include "../../LuaEngine/CLuaEngine.h"

#include "../../fx/CParticleManager.h"

//const AkGameObjectID GAME_OBJECT_PISTOLSHOT = 100;
const AkGameObjectID Ak_FLAMETHROWER = 15;

CFlamethrowerState::CFlamethrowerState()
{
	m_fFiringRate = 0.00f; //Setting the firing rate of the flamethrower.
	m_nWeaponType = FLAMETHROWER;
	m_bAudioPlayFlameNorm = false;
	m_pMesh = &CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("FlameThrowerShape"))->GetMesh();

	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Weapons/Flamethrower.lua");

	//CWwiseManager::GetInstance()->RegisterObject(GAME_OBJECT_PISTOLSHOT, "PistolSFX");
	CWwiseManager::GetInstance()->RegisterObject(Ak_FLAMETHROWER, "Flamethrower");
}

CFlamethrowerState::~CFlamethrowerState()
{
	lua_close(luaState);
}

CFlamethrowerState* CFlamethrowerState::GetInstance()
{
	static CFlamethrowerState instance;
	return &instance;
}
void CFlamethrowerState::Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType)
{
	D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
	D3DXVECTOR3 hardCodedGunTip(0.0f, 5.0f, -17.0f);

	D3DXVECTOR4 realPos;
	D3DXVec3Transform(&realPos, &hardCodedGunTip, &gunMat);
	pos.x = realPos.x;
	pos.y = realPos.y;
	pos.z = realPos.z;

	switch(shotType)
	{
	case NORMAL_FIRE:
		{
			CFlamethrowerBullet* bullet = (CFlamethrowerBullet*)(CObjectFactory::GetInstance()->GetObject<CFlamethrowerBullet>(OBJ_FLAME));

			bullet->SetType(OBJ_FLAME);
			bullet->SetOwner(owner);
			bullet->SetPosition(pos);
			D3DXVECTOR3 vel(0.0f, 0.0f, -100.0f);
			bullet->SetVelocity(Helpers::MultiplyMatrixVector(vel, gunMat));
			bullet->SetIsActive(true);
			bullet->SetIsOnGround(false);
			bullet->SetAimDir(direction);
			bullet->SetIsLeft(isLeft);

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3, 4, 0);

			D3DXMATRIX identMat;
			D3DXMatrixIdentity(&identMat);
			bullet->SetParticleEffect(CParticleManager::GetInstance()->SpawnParticle("Flamethrower_nrm2", identMat));

			//bullet->SetVelocity((float)lua_tonumber(luaState, -2), (float)lua_tonumber(luaState, -1), 0.0f);
			bullet->SetDamage((float)lua_tonumber(luaState, -3));
			bullet->SetMaxLife((float)lua_tonumber(luaState, -4));
			CObjectManager::GetInstance()->AddObject(bullet);

			lua_pop(luaState, 4);
			CFlamethrowerState::GetInstance()->SetIsPlayFlameNorm(false);

			//if(m_bAudioPlayFlameNorm == false)
			//{
			//	if(owner->GetType() == OBJ_BARRON)
			//	{
			//		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FLAMETHROWER_LOOP, 4); // Playing the sound.  Pew pew!
			//		m_bAudioPlayFlameNorm = true;
			//	}
			//	else if(owner->GetType() == OBJ_CLAIRE)
			//	{
			//		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FLAMETHROWER_LOOP, 5); // Playing the sound.  Pew pew!
			//		m_bAudioPlayFlameNorm = true;
			//	}
			//}
			break;
		}
	case ALTERNATE_FIRE:
		{
			CFlamethrowerAlternate* bullet = (CFlamethrowerAlternate*)(CObjectFactory::GetInstance()->GetObject<CFlamethrowerAlternate>(OBJ_FLAME_ALTERNATE));

			bullet->SetType(OBJ_FLAME_ALTERNATE);
			bullet->SetOwner(owner);
			bullet->SetPosition(pos);
			bullet->SetIsActive(true);
			bullet->SetIsOnGround(false);

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3, 9, 0);

			bullet->SetVelocity((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f);
			bullet->SetRotationY((float)lua_tonumber(luaState, -2));
			bullet->SetRotationZ((float)lua_tonumber(luaState, -1));
			bullet->SetDamage((float)lua_tonumber(luaState, -5));
			bullet->SetMaxLife((float)lua_tonumber(luaState, -6));
			bullet->SetNapalmDamage((float)lua_tonumber(luaState, -8));
			bullet->SetNapalmFrequency((float)lua_tonumber(luaState, -7));
			bullet->SetNapalmLife((float)lua_tonumber(luaState, -9));
			CObjectManager::GetInstance()->AddObject(bullet);

			lua_pop(luaState, 9);

			if(owner->GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_FLAMETHROWER_ALT, 150); // Playing the sound.  Pew pew!
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_FLAMETHROWER_ALT, 150); // Playing the sound.  Pew pew!
			}
			break;
		}
	}
}
void CFlamethrowerState::FireUltimate(CBase* owner, D3DXVECTOR3 pos)
{
	CFlamethrowerUltimate* bullets[2];
	for(unsigned int x = 0; x < 2; x++)
	{
		bullets[x] = (CFlamethrowerUltimate*)(CObjectFactory::GetInstance()->GetObject<CFlamethrowerUltimate>(OBJ_FLAME_ULTIMATE));
		bullets[x]->SetType(OBJ_FLAME_ULTIMATE);
		bullets[x]->SetOwner(owner);
		bullets[x]->SetPosition(pos);
		bullets[x]->SetIsActive(true);
	}

	lua_getglobal(luaState, "GetBulletInfo");
	lua_pushnumber(luaState, 0);
	lua_pushboolean(luaState, true);
	lua_pushnumber(luaState, ULTIMATE_FIRE);

	lua_pcall(luaState, 3, 9, 0);

	bullets[0]->SetIsAffectedByGrav(true);
	bullets[0]->SetDamage((float)lua_tonumber(luaState, -9));
	bullets[0]->SetVelocity((float)lua_tonumber(luaState, -8), (float)lua_tonumber(luaState, -7), 0.0f);
	bullets[0]->SetRotationZ((float)lua_tonumber(luaState, -6));
	bullets[0]->SetRotationY((float)lua_tonumber(luaState, -5));
	bullets[0]->SetAudioID(151);
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	bullets[0]->SetEffect(CParticleManager::GetInstance()->SpawnParticle("Flamethrower_ult", iden, (D3DXMATRIX*)&bullets[0]->GetFrame().GetWorldMatrix()));

	bullets[1]->SetIsAffectedByGrav(true);
	bullets[1]->SetDamage((float)lua_tonumber(luaState, -9));
	bullets[1]->SetVelocity((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f);
	bullets[1]->SetRotationZ((float)lua_tonumber(luaState, -2));
	bullets[1]->SetRotationY((float)lua_tonumber(luaState, -1));
	bullets[1]->SetAudioID(152);
	
	bullets[1]->SetEffect(CParticleManager::GetInstance()->SpawnParticle("Flamethrower_ult", iden, (D3DXMATRIX*)&bullets[1]->GetFrame().GetWorldMatrix()));

	for(unsigned int x = 0; x < 2; x++)
	{
		CObjectManager::GetInstance()->AddObject(bullets[x]);
	}

	lua_pop(luaState, 9);

	if(owner->GetType() == OBJ_BARRON)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FLAMETHROWER_ULTIMATE_SHOT, 151); // Playing the sound.  Pew pew!
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_FLAMETHROWERULT, 4);
	}
	else if(owner->GetType() == OBJ_CLAIRE)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FLAMETHROWER_ULTIMATE_SHOT, 151); // Playing the sound.  Pew pew!
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_FLAMETHROWERULT, 5);
	}

	CCamera::GetInstance()->ShakeCamera(2.0f);
}

float CFlamethrowerState::GetFiringRate(void)
{
	lua_getglobal(luaState, "GetFlamethrowerData");
	lua_pushstring(luaState, "NormalFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

float CFlamethrowerState::GetAlternateRate(void)
{
	lua_getglobal(luaState, "GetFlamethrowerData");
	lua_pushstring(luaState, "AlternateFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

int CFlamethrowerState::GetMaxAmmo(void)
{
	lua_getglobal(luaState, "GetFlamethrowerData");
	lua_pushstring(luaState, "MaxAmmo");
	lua_pcall(luaState, 1, 1, 0);

	int ammo = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return ammo;
}

int CFlamethrowerState::GetAlternateCost(void)
{
	lua_getglobal(luaState, "GetFlamethrowerData");
	lua_pushstring(luaState, "AltCost");
	lua_pcall(luaState, 1, 1, 0);

	int cost = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return cost;
}