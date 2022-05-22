#ifndef _IBASEWEAPONSTATE_H
#define _IBASEWEAPONSTATE_H

//////////////////////////////////////////////////////////////////////
// Name: IBaseWeaponState
//
// Author: TJ "8 Bit" Farnie
//
// Description: This interface will provide the methods that all the 
//				different weapon states must have and define.
//////////////////////////////////////////////////////////////////////

#include "D3DX9math.h"
#include "../../Mesh/CMesh.h"

//Enum for the various directions you can fire.
enum aim_dir{STRAIGHT_UP = 0, ANGLE_UP, STRAIGHT_FORWARD, ANGLE_DOWN, STRAIGHT_DOWN, FSU, FSD, USB, USF, DSB, DSF, UMSU, UMSD, DMSU, DMSD};
enum shot_type{NORMAL_FIRE = 0, ALTERNATE_FIRE, ULTIMATE_FIRE};
enum weapon_type{MACHINE_GUN = 0, SHOTGUN, FLAMETHROWER, LIGHTNING_GUN, ROCKET_LAUNCHER};

//Forward declarations
class CBase;

class IBaseWeaponState
{
public:
	virtual void Fire(CBase* owner, D3DXVECTOR3 pos, bool isLeft, int direction, int shotType) = 0;
	virtual void FireUltimate(CBase* owner, D3DXVECTOR3 pos) = 0;
	virtual float GetFiringRate(void) = 0;
	virtual float GetAlternateRate(void) = 0;
	virtual int GetMaxAmmo(void) = 0;
	virtual int GetAlternateCost(void) = 0;
	virtual int GetWeaponType(void) = 0;
	virtual CMesh* GetMesh(void) = 0;
};

#endif