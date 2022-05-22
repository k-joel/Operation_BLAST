#include "CSmartLaserBullet.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Camera/Camera.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"


CSmartLaserBullet::CSmartLaserBullet()
{
	SetType(OBJ_SMARTLASER);
	despawnTimer = 0.0f;
	SetIsAffectedByGrav(false);
}

CSmartLaserBullet::CSmartLaserBullet(const CSmartLaserBullet& blt)
{
}

CSmartLaserBullet::~CSmartLaserBullet()
{
}

void CSmartLaserBullet::Update(float fDt)
{
	CBase::Update(fDt);
	static float fTimer = 0.0f;
	static float fTimer2 = 0.0f;
	fTimer += fDt;
	fTimer2 += fDt;

	if(fTimer > 1.0f)
	{
		D3DXVECTOR3 vTempor;
		D3DXVec3Normalize(&vTempor,&(((CCharacter*)m_pOwner)->GetCurrentTarget()->GetPosition() - GetPosition()));
		SetVelocity(D3DXVECTOR3(200.0f * vTempor.x,100.0f * vTempor.y,0 * vTempor.z));
		fTimer = 0.0f;
	}
	if(fTimer2 > 3.0f)
	{
		SetIsActive(false);
		fTimer2 = 0.0f;
	}

	if(GetIsOnGround() == true)
	{
		SetIsActive(false);
	}

}

void CSmartLaserBullet::Render()
{

	CBase::Render();
}

bool CSmartLaserBullet::CheckCollision(CBase* pBase)
{

	return CBase::CheckCollision(pBase);
}

void CSmartLaserBullet::CollisionReact(CBase* pBase)
{

	switch(pBase->GetType())
	{
	case OBJ_GROUND:
		{
			SetIsActive(false);
			break;
		}
	}
}

//D3DXVECTOR3 pos = m_pOwner->GetPosition();