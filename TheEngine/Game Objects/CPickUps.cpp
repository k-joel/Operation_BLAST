#include "CPickUps.h"
#include "Weapon States/CShotgunState.h"
#include "Weapon States/CFlamethrowerState.h"
#include "Weapon States/CRocketState.h"
#include "Weapon States/CLightningGunState.h"

#include "../Rendering/CRenderContext.h"

//////////////////////////////////////////////////////////////////////////////////
//
// Base Pickup Class
//
//////////////////////////////////////////////////////////////////////////////////
CPickUp::CPickUp()
{
	SetType(OBJ_PICKUP);
}

CPickUp::~CPickUp()
{
}

IBaseWeaponState* CPickUp::GetWeaponState()
{
	return state;
}

void CPickUp::SetWeaponState(IBaseWeaponState* newState)
{
	state = newState;
}

void CPickUp::Initialize()
{
	CBase::Initialize();
	m_fLifeSpan = 2.0f;
	SetRenderType(RT_OPAQUE);
	SetVelocity( D3DXVECTOR3(0.0f, 0.0f, 0.0f) );
	SetAcceleration(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void CPickUp::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	m_fLifeSpan -= fElapsedTime;

	if(m_fLifeSpan < 0.0f)
	{
		SetRenderType((sin(m_fLifeSpan*m_fLifeSpan*4.0f)+0.2f<0.0f)?RT_DONOTRENDER:RT_OPAQUE);
	}

	if(m_fLifeSpan < -3.0f)
	{
		SetIsActive(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
// ShotGun Pickup
//
//////////////////////////////////////////////////////////////////////////////////
CShotGunPickUp::CShotGunPickUp()
{
	SetType(OBJ_SHOTGUN_PICKUP);
	SetWeaponState(CShotgunState::GetInstance());
}

CShotGunPickUp::~CShotGunPickUp()
{
}


bool CShotGunPickUp::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CShotGunPickUp::CollisionReact(CBase *pBase)
{

}

void CShotGunPickUp::Initialize()
{
	CPickUp::Initialize();
	SetWeaponState(CShotgunState::GetInstance());
}

//////////////////////////////////////////////////////////////////////////////////
//
// Flame Thrower Pickup
//
//////////////////////////////////////////////////////////////////////////////////
CFlameThrowerPickUp::CFlameThrowerPickUp()
{
	SetType(OBJ_FLAMETHROWER_PICKUP);
	SetWeaponState(CFlamethrowerState::GetInstance());
}

CFlameThrowerPickUp::~CFlameThrowerPickUp()
{
}


bool CFlameThrowerPickUp::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CFlameThrowerPickUp::CollisionReact(CBase *pBase)
{
}

void CFlameThrowerPickUp::Initialize()
{
	CPickUp::Initialize();
	SetWeaponState(CFlamethrowerState::GetInstance());
}

//////////////////////////////////////////////////////////////////////////////////
//
// RocketLauncher Pickup
//
//////////////////////////////////////////////////////////////////////////////////
CRocketLauncherPickUp::CRocketLauncherPickUp()
{
	SetType(OBJ_ROCKET_PICKUP);
	SetWeaponState(CRocketLauncherState::GetInstance());
}

CRocketLauncherPickUp::~CRocketLauncherPickUp()
{
}


bool CRocketLauncherPickUp::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CRocketLauncherPickUp::CollisionReact(CBase *pBase)
{
}

void CRocketLauncherPickUp::Initialize()
{
	CPickUp::Initialize();
	SetWeaponState(CRocketLauncherState::GetInstance());
}

//////////////////////////////////////////////////////////////////////////////////
//
// Lightning Gun Pickup
//
//////////////////////////////////////////////////////////////////////////////////
CLightningGunPickUp::CLightningGunPickUp()
{
	SetType(OBJ_LIGHTNING_PICKUP);
	SetWeaponState(CLightningGunState::GetInstance());
}

CLightningGunPickUp::~CLightningGunPickUp()
{
}


bool CLightningGunPickUp::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CLightningGunPickUp::CollisionReact(CBase *pBase)
{
}

void CLightningGunPickUp::Initialize()
{
	CPickUp::Initialize();
	SetWeaponState(CLightningGunState::GetInstance());
}