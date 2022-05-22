#include "CLaserBullet.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Camera/Camera.h"

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

#include "../../FX/CParticleManager.h"

//const AkGameObjectID Ak_MGUN_B = 10;

CLaserBullet::CLaserBullet()
{
	SetType(OBJ_MACHINE_GUN_BULLET);
	despawnTimer = 0.0f;
	SetIsAffectedByGrav(false);
	//CWwiseManager::GetInstance()->RegisterObject(Ak_MGUN_B, "Machine Gun Bullet");
}

CLaserBullet::CLaserBullet(const CLaserBullet& blt)
{
}

CLaserBullet::~CLaserBullet()
{
}

void CLaserBullet::Update(float fDt)
{
	//GetAnimCtrl().update(fDt);
	CBase::Update(fDt);
	//SetVelocity(D3DXVECTOR3(50.0,0.0,0.0));

	despawnTimer += fDt;

	if(despawnTimer > 1.5f)
	{
		SetIsActive(false);
		despawnTimer = 0.0f;
	}

	if(GetIsOnGround() == true)
	{
		SetIsActive(false);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BULLET_HIT_GROUND, 200);
	}
	
	D3DXMATRIX rotata;
	D3DXMatrixRotationX(&rotata, despawnTimer*50.0f);

	//correct direction and forward
	D3DXVECTOR3 nDir, curZ, newY;
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	curZ = D3DXVECTOR3(newOrien._31, newOrien._32, newOrien._33);
	nDir = GetVelocity();
	D3DXVec3Normalize(&nDir, &nDir);

	D3DXVec3Cross(&newY, &nDir, &curZ);
	D3DXVec3Normalize(&newY, &newY);

	newOrien._11 = nDir.x;
	newOrien._12 = nDir.y;
	newOrien._13 = nDir.z;

	newOrien._21 = newY.x;
	newOrien._22 = newY.y;
	newOrien._23 = newY.z;

	GetFrame().GetLocalMatrix() = rotata*newOrien;

	//CParticleManager::GetInstance()->SpawnParticle("Laser_charge", GetFrame().GetWorldMatrix());
}

void CLaserBullet::Render()
{
	/*D3DXMATRIX mxViewProj(CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix());
	D3DXCreateBox(CDirect3D::GetInstance()->GetDirect3DDevice(), 1.0f, 0.5f, 0.5f, &m_BoxMesh, NULL);
	m_BVBox.AABBFromMesh(m_BoxMesh);

	TMaterial red;
	red.m_Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	LPD3DXEFFECT effect = CShaderManager::GetInstance()->GetEffect(m_nFlatID);
	CRenderer::GetInstance()->FlatRenderMesh(m_BoxMesh, effect, m_BoxMat, mxViewProj, red);*/

	CBase::Render();
}

bool CLaserBullet::CheckCollision(CBase* pBase)
{
	/*
	TPlane plane;
	plane.d = CCamera::GetInstance()->GetRightFrustumPlane().d;
	plane.n = D3DXVECTOR3(CCamera::GetInstance()->GetRightFrustumPlane().a, CCamera::GetInstance()->GetRightFrustumPlane().b, CCamera::GetInstance()->GetRightFrustumPlane().c);
	TAABB aabb;

	this->GetAnimCtrl().GetModel()->CloneAABB(&aabb);

	if(CCollisionTests::TestAABBPlane(aabb, plane))
	{
		SetIsActive(false);
		return false;
	}
	else
		return CBase::CheckCollision(pBase);
		*/
	return CBase::CheckCollision(pBase);
}

void CLaserBullet::CollisionReact(CBase* pBase)
{
	//The bullet will only react to hitting the ground

	switch(pBase->GetType())
	{
	case OBJ_GROUND:
		{
			SetIsActive(false);
			//Play a sound here?  WHO KNOWS!  =D
			break;
		}
	}
}


void CLaserBullet::Initialize()
{
	CBase::Initialize();
}