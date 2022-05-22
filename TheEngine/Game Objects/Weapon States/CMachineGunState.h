#ifndef _CMACHINEGUNSTATE_H
#define _CMACHINEGUNSTATE_H
//////////////////////////////////////////////////////////////////////
// Name: CMachineGunState
//
// Author: TJ "Tommy Gun" Farnie
//
// Description: This state houses all the functionality associated
//				with the machine gun.
//////////////////////////////////////////////////////////////////////

#include "IBaseWeaponState.h"
#include "../../LuaEngine/CLuaEngine.h"

//Forward declarations
class CBase;
class CMachineGunBullet;

class CMachineGunState : public IBaseWeaponState
{
private:
	lua_State* luaState; //Pointer to a lua state for this file.
	float m_fAlternateRate; //No alt, ignore this.
	int m_nAltCost; //Alternate cost for this weapon.  Since this is default, there is none.  Set this in the constructor.
	int m_nWeaponType; //Type of gun this is.  In this case, MACHINE_GUN.  Set in the constructor.
	CMesh* m_pMesh; //Mesh

	CMachineGunState();
	CMachineGunState(const CMachineGunState&);
	CMachineGunState& operator=(const CMachineGunState&);
	~CMachineGunState();

	//Helper method to fill out the bullet.
	void SetupBullet(CMachineGunBullet* bullet, int direction, bool isLeft);
public:
	void Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType);
	void FireUltimate(CBase* owner, D3DXVECTOR3 pos);

	static CMachineGunState* GetInstance(void);

	//Accessors
	float GetAlternateRate(void) { return m_fAlternateRate; }
	int GetAlternateCost(void) { return m_nAltCost; }
	int GetWeaponType(void) { return m_nWeaponType; }
	CMesh* GetMesh(void) { return m_pMesh; }

	//Mutators

	//Getting values from Lua
	float GetFiringRate(void);
	int GetMaxAmmo(void);
};

#endif