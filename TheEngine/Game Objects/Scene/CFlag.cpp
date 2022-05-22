#include "CFlag.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Collision/CollisionObject.h"
#include "../../Spacial Tree/CBVH.h"

#include "../../FX/CParticleManager.h"

#include "../../Event System/CEventSystem.h"

#include "../../Camera/Camera.h"

#include "../CObjectFactory.h"
#include "../CObjectManager.h"
#include "../CDebris.h"

CFlag::CFlag()
{
	SetType(OBJ_FLAG);
	SetIsAffectedByGrav(false);
	m_fLifeTime = 0.0f;
	m_fWinTimer = -1.0f;
}

CFlag::~CFlag() {}

void CFlag::Initialize()
{
	m_fLifeTime = 0.0f;
	m_fWinTimer = -1.0f;
}

void CFlag::Update(float fElapsedTime)
{

	//animation & particles here
	m_fLifeTime += fElapsedTime;

	D3DXMATRIX rotUpdate, offset;

	//top blade
	D3DXMatrixRotationY(&rotUpdate, m_fLifeTime*10.0f);
	D3DXMatrixTranslation(&offset, 0.0f, 12.6f, 40.0f);
	GetFrameAt(1).GetLocalMatrix() = rotUpdate * offset;

	//back blade
	D3DXMatrixRotationX(&rotUpdate, m_fLifeTime*10.0f);
	D3DXMatrixTranslation(&offset, -7.0f, 22.0f, -100.0f);
	GetFrameAt(2).GetLocalMatrix() = rotUpdate * offset;

	//wheels
	D3DXVECTOR3 vel = GetVelocity();
	float len = D3DXVec3Length(&vel);
	if(len > 50)
	{
		SetVelocity(vel*(1/len)*50.0f);
	}

	SetVelocity(GetVelocity()*0.9999f + 0.1*(m_vgoal - GetPosition()));

	CBase::Update(fElapsedTime);

	if(CCollisionTests::IntersectFrustrumSphere(m_Sphere.GetSphere(), CCamera::GetInstance()->GetFrustum()) == CT_IN)
	{
		float offset;
		D3DXMATRIX randOffset, randRotation;

		offset = (float)(rand()%1000)/40.0f;
		D3DXMatrixTranslation(&randOffset, offset, 0.0f,offset);
		D3DXMatrixRotationY(&randRotation, offset);

		CBase RealBox;
		RealBox.GetBVAABB().GetAABB().maxPt = D3DXVECTOR3(GetPosition().x+10,200,200);
		RealBox.GetBVAABB().GetAABB().minPt = D3DXVECTOR3(GetPosition().x-10,-200,-200);;

		D3DXVECTOR3 vF;
		float fDist;
		D3DXVECTOR3 start = D3DXVECTOR3(GetPosition().x,RealBox.GetBVAABB().GetAABB().maxPt.y,0);
		D3DXVECTOR3 direction = D3DXVECTOR3(0,-1,0);

		vector<CBase*> returnedObjs;
		CBVH::GetInstance()->TraverseCollisions(CBVH::GetInstance()->GetRootCollisionNode(), &RealBox, &returnedObjs);
		if(returnedObjs.size())
		{
			for(unsigned int x=0; x < returnedObjs.size(); x++)
			{
				if(CCollisionTests::TestRayMesh(start, direction, returnedObjs[x]->GetShape(0).GetMesh(),returnedObjs[x]->GetFrame().GetWorldMatrix(), vF, fDist))
				{

					D3DXMATRIX identMat;
					D3DXMatrixIdentity(&identMat);
					identMat._41 = vF.x;
					identMat._42 = vF.y+3.0f;
					identMat._43 = vF.z;

					CParticleManager::GetInstance()->SpawnParticle("Assaulter_land", randOffset * randRotation * identMat);
					break;
				}
			}
		}
	}

	//timer for winning here
	if(m_fWinTimer > 0.0f)
	{
		m_fWinTimer += fElapsedTime;
	}
	
	if(m_fWinTimer > 15.0f)
	{
		m_fWinTimer = -1.0f;
		char* buffer = "whatever";
		CEvent* test = CEventSystem::GetInstance()->FindEvent("WinEvent");
		CEventSystem::GetInstance()->DispatchEvent(*test, NULL, 2.0f);
		SetIsActive(false);
	}
}