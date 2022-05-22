
// Includes
#include "CRocketState.h"
#include "../CBase.h"
#include "../Bullet/CRocketBullet.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"
#include "../../LuaEngine/CLuaEngine.h"
#include "../../Camera/Camera.h"
#include "../../Collision/CCollisionTests.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

CRocketLauncherState::CRocketLauncherState()
{
	m_nWeaponType = ROCKET_LAUNCHER;
	m_pMesh = &CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("RocketLauncherShape"))->GetMesh();

	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Weapons/RocketLauncher.lua");

}

CRocketLauncherState::~CRocketLauncherState()
{
	lua_close(luaState);
}

CRocketLauncherState* CRocketLauncherState::GetInstance(void)
{
	static CRocketLauncherState instance;
	return &instance;
}

void CRocketLauncherState::Fire(CBase *owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType)
{
	D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
	D3DXVECTOR3 hardCodedGunTip(0.0f, 5.0f, -20.0f);

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
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER, 4);
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER, 5);
			}

			CRocketBullet* bullet = (CRocketBullet*)(CObjectFactory::GetInstance()->GetObject<CRocketBullet>(OBJ_ROCKET_BULLET));

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3,6, 0);

			bullet->SetPosition(pos);
			bullet->SetMaxLife((float)lua_tonumber(luaState, -6));
			bullet->SetIsActive(true);
			bullet->SetDamage((float)lua_tonumber(luaState, -5));
			bullet->SetOwner(owner);
			bullet->SetVelocity(D3DXVECTOR3((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f));
			bullet->SetRotationY((float)lua_tonumber(luaState, -2));
			bullet->SetRotationZ((float)lua_tonumber(luaState, -1));
			bullet->SetType(OBJ_ROCKET_BULLET);

			CObjectManager::GetInstance()->AddObject(bullet);			
			
			lua_pop(luaState, 6);

			break;
		}
	case ALTERNATE_FIRE:
		{
			if(owner->GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER, 4);
			}
			else if(owner->GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER, 5);
			}

			CRocketAlternate* bullet = (CRocketAlternate*)(CObjectFactory::GetInstance()->GetObject<CRocketAlternate>(OBJ_ROCKET_ALTERNATE));

			lua_getglobal(luaState, "GetBulletInfo");
			lua_pushnumber(luaState, direction);
			lua_pushboolean(luaState, isLeft);
			lua_pushnumber(luaState, shotType);

			lua_pcall(luaState, 3,6, 0);

			bullet->SetPosition(pos);
			bullet->SetMaxLife((float)lua_tonumber(luaState, -6));
			bullet->SetIsActive(true);
			bullet->SetDamage((float)lua_tonumber(luaState, -5));
			bullet->SetOwner(owner);
			bullet->SetVelocity(D3DXVECTOR3((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f));
			bullet->SetRotationY((float)lua_tonumber(luaState, -2));
			bullet->SetRotationZ((float)lua_tonumber(luaState, -1));
			bullet->SetType(OBJ_ROCKET_ALTERNATE);

			CObjectManager::GetInstance()->AddObject(bullet);			

			lua_pop(luaState, 6);

			break;
		}
	case ULTIMATE_FIRE:
		{
			break;
		}
	};

}

void CRocketLauncherState::FireUltimate(CBase* owner, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 center;
	center.x = CCamera::GetInstance()->GetViewPosition().x;
	center.y = 350;
	center.z = owner->GetPosition().z;

	if(owner->GetType() == OBJ_BARRON)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER, 4);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_ROCKETLAUNCHERULT, 4);
	}
	else if(owner->GetType() == OBJ_CLAIRE)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEAPONS_ROCKETLAUNCHER, 5);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_ROCKETLAUNCHERULT, 5);
	}
	


	int XStart;
	int XOffset;
	if(CCamera::GetInstance()->GetViewPosition().z < -300)
	{
		XStart = (int)center.x - 165;
		XOffset = 50;
	}
	else
	{
		XStart = (int)center.x - 140;
		XOffset = 40;
	}
	int YOffset = 30;

	D3DXVECTOR3 posi;

	int miss = rand() % 8;
	CRocketUltimate* bullets[8];
	for(unsigned int i = 0; i < 8; i++)
	{
		if(i != miss)
		{
			posi.x = (float)XStart;
			posi.y = center.y;// + YOffset;
			posi.z = 0;
			bullets[i] = (CRocketUltimate*)(CObjectFactory::GetInstance()->GetObject<CRocketUltimate>(OBJ_ROCKET_ULTIMATE));
			bullets[i]->SetMaxLife(20.0f);
			bullets[i]->SetDamage(50.0f);
			bullets[i]->SetIsActive(true);
			bullets[i]->SetOwner(owner);
			bullets[i]->SetType(OBJ_ROCKET_ULTIMATE);
			bullets[i]->SetPosition(posi);
			CObjectManager::GetInstance()->AddObject(bullets[i]);
		}
		
		XStart += XOffset;
		YOffset += 20;
	}
}

int CRocketLauncherState::GetMaxAmmo(void)
{
	lua_getglobal(luaState, "GetRocketLauncherData");
	lua_pushstring(luaState, "MaxAmmo");
	lua_pcall(luaState, 1, 1, 0);

	int ammo = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return ammo;
}

float CRocketLauncherState::GetFiringRate(void)
{
	lua_getglobal(luaState, "GetRocketLauncherData");
	lua_pushstring(luaState, "NormalFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

float CRocketLauncherState::GetAlternateRate(void)
{
	lua_getglobal(luaState, "GetRocketLauncherData");
	lua_pushstring(luaState, "AlternateFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

int CRocketLauncherState::GetAlternateCost(void)
{
	lua_getglobal(luaState, "GetRocketLauncherData");
	lua_pushstring(luaState, "AltCost");
	lua_pcall(luaState, 1, 1, 0);

	int cost = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return cost;
}