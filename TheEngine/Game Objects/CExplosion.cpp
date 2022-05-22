#include "CExplosion.h"


CExplosionObject::CExplosionObject()
{
	SetType(OBJ_EXPLOSION);
	SetIsAffectedByGrav(false);
}

CExplosionObject::~CExplosionObject()
{

}

void CExplosionObject::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	GetShape(0).GetAnimCtrl().update(fElapsedTime);
	GetShape(1).GetAnimCtrl().update(fElapsedTime);
	GetShape(2).GetAnimCtrl().update(fElapsedTime);
	GetShape(3).GetAnimCtrl().update(fElapsedTime);

	m_lifeTime -= fElapsedTime;

	if(m_lifeTime < 0.0f)
	{
		SetIsActive(false);
	}
}

void CExplosionObject::Render()
{
}

bool CExplosionObject::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CExplosionObject::CollisionReact(CBase* pBase)
{
}

void CExplosionObject::SetupExplosion(CBase* creator, float lifeTime)
{
	m_creator = creator;
	m_lifeTime = lifeTime;
	SetIsActive(true);
}