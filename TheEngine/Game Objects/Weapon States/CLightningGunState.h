#ifndef _CLIGHTNINGGUNSTATE_H_
#define _CLIGHTNINGGUNSTATE_H_

// Includes
#include "IBaseWeaponState.h"
#include "../../LuaEngine/CLuaEngine.h"

// Forward Declarations
class CBase;
//class CRocketBullet;

// Class Definition
class CLightningGunState	:	public IBaseWeaponState
{
// Private
private:

	lua_State* luaState;
	float m_fFireRate;
	float m_fAlternateRate;
	int m_nMaxAmmo;
	int m_nAltCost;
	int m_nWeaponType;
	CMesh* m_pMesh;

	CLightningGunState();
	CLightningGunState(const CLightningGunState&);
	CLightningGunState& operator=(const CLightningGunState&);
	~CLightningGunState();

//Public	
public:

	void	Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType);
	void	FireUltimate(CBase* owner, D3DXVECTOR3 pos);

	static CLightningGunState* GetInstance(void);

	// Accessors
	int		GetWeaponType(void)		{ return m_nWeaponType; }
	CMesh*	GetMesh(void)			{ return m_pMesh; }

	//Mutators

	//Additional functions
	float	GetFiringRate(void);
	float	GetAlternateRate(void);
	int		GetMaxAmmo(void);
	int		GetAlternateCost(void);
};

#endif