#include "CMachineGunState.h"
#include "../CBase.h"
#include "../Bullet/CMachineGunBullet.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

#include "../../LuaEngine/CLuaEngine.h"

#include "../../FX/CFXManager.h"

//const AkGameObjectID GAME_OBJECT_PISTOLSHOT = 100;

CMachineGunState::CMachineGunState()
{
	m_fAlternateRate = 0.0f;
	m_nWeaponType = MACHINE_GUN;

	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Weapons/MachineGun.lua");
}

CMachineGunState::~CMachineGunState()
{
	lua_close(luaState);
}

CMachineGunState* CMachineGunState::GetInstance()
{
	static CMachineGunState instance;
	return &instance;
}
void CMachineGunState::Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType)
{
	//place bullet at end of gun
	D3DXVECTOR3 hardCodedGunTip;
	if(owner->GetType() == OBJ_BARRON)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MACHINEGUN, 4); // Playing the sound.  Pew pew!

		D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
		hardCodedGunTip = D3DXVECTOR3(0.0f, 5.0f, -17.0f);

		D3DXVECTOR4 realPos;
		D3DXVec3Transform(&realPos, &hardCodedGunTip, &gunMat);
		pos.x = realPos.x;
		pos.y = realPos.y;
		pos.z = realPos.z;
	}
	else if(owner->GetType() == OBJ_CLAIRE)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MACHINEGUN, 5); // Playing the sound.  Pew pew!

		D3DXMATRIX gunMat = owner->GetFrameAt(1).GetWorldMatrix();
		hardCodedGunTip = D3DXVECTOR3(0.0f, 2.5f, -4.0f);

		D3DXVECTOR4 realPos;
		D3DXVec3Transform(&realPos, &hardCodedGunTip, &gunMat);
		pos.x = realPos.x;
		pos.y = realPos.y;
		pos.z = realPos.z;
	}

	CMachineGunBullet* bullet = (CMachineGunBullet*)(CObjectFactory::GetInstance()->GetObject<CMachineGunBullet>(OBJ_MACHINE_GUN_BULLET));

	bullet->SetPosition(pos);
	bullet->SetIsActive(true);
	bullet->SetOwner(owner);
	bullet->SetType(OBJ_MACHINE_GUN_BULLET);

	SetupBullet(bullet, direction, isLeft);

	//muzzle flash
	D3DXMATRIX rot, trans, muzzleMat = owner->GetFrameAt(1).GetWorldMatrix();
	D3DXMatrixRotationY(&rot, D3DX_PI/2.0f);
	D3DXMatrixTranslation(&trans, hardCodedGunTip.x, hardCodedGunTip.y,(owner->GetVelocity().z != 0.0f)?( hardCodedGunTip.z-13.0f):( hardCodedGunTip.z-10.0f));
	int id = CFXManager::GetInstance()->LoadEffect(FX_MGUN_MUZZLEFLASH, rot * trans * muzzleMat, 0.05f, false );

	CObjectManager::GetInstance()->AddObject(bullet);
}

void CMachineGunState::FireUltimate(CBase* owner, D3DXVECTOR3 pos)
{
}

void CMachineGunState::SetupBullet(CMachineGunBullet* bullet, int direction, bool isLeft)
{
	float spread = (rand() % 40) - 20.0f; // Spread of the bullet, locked between 20 and -20.  Or something like that.

	lua_getglobal(luaState, "GetBulletInfo");
	lua_pushnumber(luaState, direction);
	lua_pushboolean(luaState, isLeft);

	lua_pcall(luaState, 2, 6, 0);

	bullet->SetRotationY((float)lua_tonumber(luaState, -1));
	bullet->SetRotationZ((float)lua_tonumber(luaState, -2));
	bullet->SetVelocity((float)lua_tonumber(luaState, -4), (float)lua_tonumber(luaState, -3), 0.0f);
	bullet->SetDamage((float)lua_tonumber(luaState, -5));
	bullet->SetMaxLife((float)lua_tonumber(luaState, -6));
	lua_pop(luaState, 6);
}

float CMachineGunState::GetFiringRate(void)
{
	lua_getglobal(luaState, "GetMachineGunData");
	lua_pushstring(luaState, "NormalFiringRate");
	lua_pcall(luaState, 1, 1, 0);

	float rate = (float)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return rate;
}

int CMachineGunState::GetMaxAmmo(void)
{
	lua_getglobal(luaState, "GetMachineGunData");
	lua_pushstring(luaState, "MaxAmmo");
	lua_pcall(luaState, 1, 1, 0);

	int ammo = (int)lua_tonumber(luaState, -1);

	lua_pop(luaState, 1);
	return ammo;
}