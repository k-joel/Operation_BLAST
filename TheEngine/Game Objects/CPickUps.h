#ifndef CPICKUPS_H
#define CPICKUPS_H

#include "CBase.h"
#include "Weapon States/IBaseWeaponState.h"

//////////////////////////////////////////////////////////////////////////////////
//
// Base Pickup Class
//
//////////////////////////////////////////////////////////////////////////////////
class CPickUp : public CBase
{
private:
	IBaseWeaponState* state;
	int m_nEffectID;
	float m_fLifeSpan;

public:
	CPickUp();
	~CPickUp();

	IBaseWeaponState* GetWeaponState();
	void SetWeaponState(IBaseWeaponState* newState);
	void Initialize();

	void Update(float fElapsedTime);
};

//////////////////////////////////////////////////////////////////////////////////
//
// ShotGun PickUp
//
//////////////////////////////////////////////////////////////////////////////////
class CShotGunPickUp : public CPickUp
{
private:

public:
	CShotGunPickUp();
	~CShotGunPickUp();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);
	void Initialize();

};

//////////////////////////////////////////////////////////////////////////////////
//
// Flame Thrower PickUp
//
//////////////////////////////////////////////////////////////////////////////////
class CFlameThrowerPickUp : public CPickUp
{
private:

public:
	CFlameThrowerPickUp();
	~CFlameThrowerPickUp();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase); 
	void Initialize();

};

//////////////////////////////////////////////////////////////////////////////////
//
// RocketLauncher PickUp
//
//////////////////////////////////////////////////////////////////////////////////
class CRocketLauncherPickUp : public CPickUp
{
private:

public:
	CRocketLauncherPickUp();
	~CRocketLauncherPickUp();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase); 
	void Initialize();

};

//////////////////////////////////////////////////////////////////////////////////
//
// Lightning PickUp
//
//////////////////////////////////////////////////////////////////////////////////
class CLightningGunPickUp : public CPickUp
{
private:

public:
	CLightningGunPickUp();
	~CLightningGunPickUp();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase); 
	void Initialize();

};

#endif