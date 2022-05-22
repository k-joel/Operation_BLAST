#ifndef _CFLAMETHROWERSTATE_H
#define _CFLAMETHROWERSTATE_H
//////////////////////////////////////////////////////////////////////
// Name: CShotgunState
//
// Author: TJ "Toasty" Farnie
//
// Description: This state will take care of all the functionality of
//				the flamethrower.
//////////////////////////////////////////////////////////////////////

#include "IBaseWeaponState.h"
#include "../../LuaEngine/CLuaEngine.h"

//Forward declarations
class CBase;

class CFlamethrowerState : public IBaseWeaponState
{
private:
	lua_State* luaState; //Lua state of the flamethrower

	float m_fFiringRate;  //Firing rate of the flamethrower.  Set up in the constructor
	float m_fAlternateRate; //Firing rate of the alternate.  Set up in the constructor.

	bool m_bAudioPlayFlameNorm;
	
	int m_nMaxAmmo; //Maximum ammo this weapon gives the player.  Set in the constructor.
	int m_nAltCost; //Cost for firing the alternate of this weapon.  Set in the constructor.
	int m_nWeaponType; //Type for the weapon.  In this case, SHOTGUN.  Set in the constructor.
	
	CMesh* m_pMesh; //Mesh

	//Singleton stuff
	CFlamethrowerState(void);
	CFlamethrowerState(const CFlamethrowerState&);
	CFlamethrowerState& operator=(const CFlamethrowerState&);
	~CFlamethrowerState(void);

public:
	void Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType);
	void FireUltimate(CBase* owner, D3DXVECTOR3 pos);

	static CFlamethrowerState* GetInstance(void);

	//Accessors
	int GetWeaponType(void) { return m_nWeaponType; }
	CMesh* GetMesh(void) { return m_pMesh; }
	bool GetIsPlayFlameNorm(void)	{ return m_bAudioPlayFlameNorm; }

	//Mutators
	void SetIsPlayFlameNorm( bool bPlay ) { m_bAudioPlayFlameNorm = bPlay; }

	//Additional functions
	float GetFiringRate(void);
	float GetAlternateRate(void);
	int GetMaxAmmo(void);
	int GetAlternateCost(void);
};
#endif