#include "CEffect.h"
#include "../Collision/CCollisionTests.h"
#include "../Collision/CollisionObject.h"
#include "../../Spacial Tree/CBVH.h"

#include "CParticleManager.h"

CEffect::CEffect()
{
	m_nType = OBJ_EFFECT;

	m_pOwnerFrame = NULL;
	m_bIsLooping = false;
	m_fLife = 0.0f;
	m_fCurrentLife = 0.0f;
	m_nFXType = -1;
	m_bLerpSphere = false;
	m_bLerpAABB = false;
	m_bIsAffectedByGrav = false;
}

CEffect::~CEffect()
{
}

void CEffect::Update(float fElapsedTime)
{
	if(m_nFXType == FX_DROP_TARGET && !GetIsOnGround())
	{
		CBase RealBox;
		RealBox.GetBVAABB().GetAABB().maxPt = D3DXVECTOR3(GetPosition().x+10,2000+GetPosition().y,2000+GetPosition().z);
		RealBox.GetBVAABB().GetAABB().minPt = D3DXVECTOR3(GetPosition().x-10,-2000+GetPosition().y,-2000+GetPosition().z);;

		D3DXVECTOR3 vF;
		float fDist;
		D3DXVECTOR3 start = D3DXVECTOR3(GetPosition().x,RealBox.GetBVAABB().GetAABB().maxPt.y,GetPosition().z);
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

					GetFrame().GetLocalMatrix() = identMat;
					break;
				}
			}
		}

		SetIsOnGround(true);
	}

	D3DXMATRIX orient = GetFrame().GetLocalMatrix();
	CBase::Update(fElapsedTime);
	GetFrame().GetLocalMatrix() = orient;

	//D3DXVECTOR3 realPosition(orient._41,orient._42,orient._43);

	m_AABB.Update((D3DXMATRIX&)GetFrame().GetWorldMatrix());
	m_Sphere.Update((D3DXMATRIX&)GetFrame().GetWorldMatrix());

	if(m_bHasAnimation)
	{
		for(unsigned int i = 0; i < m_Shapes.size(); i++)
			GetShape(i).GetAnimCtrl().update(fElapsedTime);
	}

	m_fCurrentLife += fElapsedTime;

	if(m_bLerpSphere)
	{
		m_StartSphere.GetSphere().c = m_EndSphere.GetSphere().c = m_Sphere.GetSphere().c;
		m_Sphere.GetSphere().r = LERP(m_StartSphere.GetSphere().r, m_EndSphere.GetSphere().r, m_fCurrentLife/m_fLife);
	}

	if(m_bLerpAABB)
	{
		m_EndAABB.Update((D3DXMATRIX&)GetFrame().GetWorldMatrix());
		m_StartAABB.GetAABB().center = m_EndAABB.GetAABB().center;

		float s = m_fCurrentLife/m_fLife;
		D3DXVec3Lerp(&m_AABB.GetAABB().minPt, &m_StartAABB.GetAABB().minPt, &m_EndAABB.GetAABB().minPt, s);
		D3DXVec3Lerp(&m_AABB.GetAABB().maxPt, &m_StartAABB.GetAABB().maxPt, &m_EndAABB.GetAABB().maxPt, s);
	}

	if(m_fLife != 0.0f && m_fCurrentLife >= m_fLife)
	{
		if(!m_bIsLooping)
		{
			SetIsActive(false);
			m_fCurrentLife = 0.0f;
		}
		else
			m_fCurrentLife = 0.0f;
	}
}