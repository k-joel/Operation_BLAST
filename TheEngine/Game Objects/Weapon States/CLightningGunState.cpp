
// Includes
#include "CLightningGunState.h"
#include "../CBase.h"
#include "../Bullet/CLightningBullet.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"
#include "../../LuaEngine/CLuaEngine.h"
#include "../CPlayer.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

CLightningGunState::CLightningGunState()
{
	m_nWeaponType = LIGHTNING_GUN;
	m_pMesh = &CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("LightningGunShape"))->GetMesh();

	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Weapons/LightningGun.lua");
}

CLightningGunState::~CLightningGunState()
{
	lua_close(luaState);
}

CLightningGunState* CLightningGunState::GetInstance(void)
{
	static CLightningGunState instance;
	return &instance;
}

void CLightningGunState::Fire(CBase *owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType)
{
	switch(shotType)
	{
	case NORMAL_FIRE:
		{

			if(owner->GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_LIGHTNINGGUN, 120);
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_LIGHTNINGGUN, 120);
			}

			D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
			D3DXVECTOR3 hardCodedGunTip(0.0f, 5.0f, -25.0f);

			D3DXVECTOR4 realPos;
			D3DXVec3Transform(&realPos, &hardCodedGunTip, &gunMat);
			pos.x = realPos.x;
			pos.y = realPos.y;
			pos.z = realPos.z;

			CLightningBullet* bullet = (CLightningBullet*)CObjectFactory::GetInstance()->GetObject<CLightningBullet>(OBJ_LIGHTNING);

			bullet->SetIsActive(true);
			bullet->SetIsOnGround(false);
			bullet->SetType(OBJ_LIGHTNING);

			//Don't forget, more stuff to do here (set owner)
			bullet->SetOwner(owner);
			bullet->SetPosition(pos);

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3, 6, -1);
			
			//Comment this crap back in once the bullets are set up to actually have these functions!

			bullet->SetMaxLife((float)lua_tonumber(luaState, -6));
			bullet->SetDamage((float)lua_tonumber(luaState, -5));
			bullet->SetVelocity(D3DXVECTOR3((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f));
			bullet->SetRotationY((float)lua_tonumber(luaState, -2));
			bullet->SetRotationZ((float)lua_tonumber(luaState, -1));
			
			lua_pop(luaState, 6);

			CObjectManager::GetInstance()->AddObject(bullet);
			break;
		}
	case ALTERNATE_FIRE:
		{	
			/*if(owner->GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_LIGHTNINGGUN, 4);
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_LIGHTNINGGUN, 5);
			}*/

			D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
			D3DXVECTOR3 hardCodedGunTip(0.0f, 5.0f, -25.0f);

			D3DXVECTOR4 realPos;
			D3DXVec3Transform(&realPos, &hardCodedGunTip, &gunMat);
			pos.x = realPos.x;
			pos.y = realPos.y;
			pos.z = realPos.z;

			CLightningAlternate* bullet = (CLightningAlternate*)CObjectFactory::GetInstance()->GetObject<CLightningAlternate>(OBJ_LIGHTNING_ALTERNATE);

			bullet->SetIsActive(true);
			bullet->SetIsOnGround(false);
			bullet->SetType(OBJ_LIGHTNING_ALTERNATE);
			bullet->SetSplitState(BASE_BOLT);
			bullet->SetIsSplitting(false);

			bullet->SetOwner(owner);
			bullet->SetPosition(pos);

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3, 6, -1);

			bullet->SetMaxLife((float)lua_tonumber(luaState, -6));
			bullet->SetDamage((float)lua_tonumber(luaState, -5));
			bullet->SetVelocity(D3DXVECTOR3((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f));
			bullet->SetRotationY((float)lua_tonumber(luaState, -2));
			bullet->SetRotationZ((float)lua_tonumber(luaState, -1));
			
			lua_pop(luaState, 6);

			CObjectManager::GetInstance()->AddObject(bullet);

			break;
			
		}
	};

}

void CLightningGunState::FireUltimate(CBase* owner, D3DXVECTOR3 pos)
{
	if(owner->GetType() == OBJ_BARRON)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_LIGHTNINGGUNULT, 4);
	}
	else if(owner->GetType() == OBJ_CLAIRE)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_LIGHTNINGGUNULT, 5);
	}

	lua_getglobal(luaState, "GetBulletInfo");
	lua_pushnumber(luaState, 0);
	lua_pushboolean(luaState, false);
	lua_pushnumber(luaState, ULTIMATE_FIRE);

	lua_pcall(luaState, 3, 5, -1);
	
	((CPlayer*)(owner))->SetLightningStorm(true);
	((CPlayer*)(owner))->SetStormBoltSpeed((float)lua_tonumber(luaState, -5));
	((CPlayer*)(owner))->SetStormDamage((float)lua_tonumber(luaState, -4));
	((CPlayer*)(owner))->SetStormDelay((float)lua_tonumber(luaState, -3));
	((CPlayer*)(owner))->SetStormDelayDecay((float)lua_tonumber(luaState, -2));
	((CPlayer*)(owner))->SetStormMaxBolts((int)lua_tonumber(luaState, -1));

	lua_pop(luaState, 5);
}

int CLightningGunState::GetMaxAmmo(void)
{
	lua_getglobal(luaState, "GetLightningGunData");
	lua_pushstring(luaState, "MaxAmmo");
	lua_pcall(luaState, 1, 1, 0);

	int ammo = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return ammo;
}

float CLightningGunState::GetFiringRate(void)
{
	lua_getglobal(luaState, "GetLightningGunData");
	lua_pushstring(luaState, "NormalFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

float CLightningGunState::GetAlternateRate(void)
{
	lua_getglobal(luaState, "GetLightningGunData");
	lua_pushstring(luaState, "AlternateFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

int CLightningGunState::GetAlternateCost(void)
{
	lua_getglobal(luaState, "GetLightningGunData");
	lua_pushstring(luaState, "AltCost");
	lua_pcall(luaState, 1, 1, 0);

	int cost = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return cost;
}