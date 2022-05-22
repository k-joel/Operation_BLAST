#ifndef _CSHOTGUNSTATE_H
#define _CSHOTGUNSTATE_H
//////////////////////////////////////////////////////////////////////
// Name: CShotgunState
//
// Author: TJ "Buckshot" Farnie
//
// Description: This state will take care of all the functionality of
//				the shotgun.
//////////////////////////////////////////////////////////////////////

#include "IBaseWeaponState.h"
#include "../../Mesh/CMesh.h"
#include "../../LuaEngine/CLuaEngine.h"

//Forward declarations
class CBase;
class CShotgunBullet;

class CShotgunState : public IBaseWeaponState
{
private:
	lua_State* luaState;
	int m_nWeaponType; //Type for the weapon.  In this case, SHOTGUN.  Set in the constructor.
	CMesh* m_pMesh; //The mesh o' the weapon

	//Singleton stuff
	CShotgunState(void);
	CShotgunState(const CShotgunState&);
	CShotgunState& operator=(const CShotgunState&);
	~CShotgunState(void);
public:
	void Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType);
	void FireUltimate(CBase* owner, D3DXVECTOR3 pos);

	static CShotgunState* GetInstance(void);

	//Accessors
	int GetWeaponType(void) { return m_nWeaponType; }
	CMesh* GetMesh(void) { return m_pMesh; }

	//Mutators

	//Other functions
	int GetMaxAmmo(void);
	float GetFiringRate(void);
	float GetAlternateRate(void);
	int GetAlternateCost(void);

};
#endif
