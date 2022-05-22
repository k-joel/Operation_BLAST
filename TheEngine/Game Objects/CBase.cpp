#include "CBase.h"
#include "..\States\CGameState.h"
#include "..\Rendering\CRenderer.h"
#include "..\Rendering\CRenderContextManager.h"
#include "..\Rendering\CRenderShape.h"
#include "..\Collision\CCollisionTests.h"
#include "../Game Objects/CObjectFactory.h"
#include "../Spacial Tree/CHGrid.h"

#include "../Collision/CCollisionResponse.h"
#include "../Debug System/DebugSystem.h"

CBase::CBase()
{
	m_uiRefCount = 0; // Obj manager will increment this when added

	SetPosition( D3DXVECTOR3(0.0f, 0.0f, 0.0f) );
	SetVelocity( D3DXVECTOR3(0.0f, 0.0f, 0.0f) );
	SetAcceleration(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetRotationY(0.0f);
	SetRotationZ(0.0f);
	SetRotationX(0.0f);
	SetSpeed(0.0f);
	SetIsEnemy(false);
	SetIsActive(true);
	SetType(OBJ_BASE);
	SetRenderType(RT_OPAQUE);
	SetHasAnimation(false);
	SetIsFacingLeft(false);
	SetIsOnGround(false);
	SetIsAffectedByGrav(true);
	SetScale(1.0f);
	SetIsOnGround(false);
	SetHitWall(false);
	m_pNextObject = NULL;
	m_nLevel = 0;
	m_nBucket = 0;

	// sets the object's bv type
	m_pBV = &m_AABB;
}

CBase::~CBase()
{
}

void CBase::Initialize()
{
	SetIsOnGround(false);
	SetHitWall(false);
	SetIsActive(true);
}

void CBase::Shutdown()
{
}

void CBase::AddRef(void)
{
	m_uiRefCount++;
}

void CBase::Release(void)
{
	m_uiRefCount--;

	if(m_uiRefCount == 0)
	{
		CObjectFactory::GetInstance()->ReturnObject(this, m_nType);
	}
}

void CBase::Update(float fElapsedTime)
{
	m_vPrevPosition = m_vPosition;
	m_vPosition += m_vVelocity * fElapsedTime;
	if(!m_bIsOnGround && m_bIsAffectedByGrav)
		m_vVelocity += g_Gravity * fElapsedTime;

	if(this->GetType() == OBJ_BARRON || this->GetType() == OBJ_CLAIRE)
	{
		if(m_vVelocity.y < 0)
		m_vVelocity += (g_Gravity * 1.3) * fElapsedTime;
	}

	D3DXMATRIX trans, rotX, rotY, rotZ, scale;

	// compute world matrix based on pos/rot
	D3DXMatrixRotationY( &rotY, m_fRotationY );
	D3DXMatrixRotationZ( &rotZ, m_fRotationZ );
	D3DXMatrixRotationX( &rotX, m_fRotationX );
	D3DXMatrixTranslation( &trans, m_vPosition.x, m_vPosition.y, m_vPosition.z );
	D3DXMatrixScaling(&scale, m_fScale, m_fScale, m_fScale );

	GetFrame().GetLocalMatrix() = rotX * rotY * rotZ * trans * scale;

	// the aabb will not be scaled unless the trans*scale matrix is passed in
	if(GetType() != OBJ_BOSSBODY)
		m_AABB.Update((D3DXMATRIX&)GetFrame().GetWorldMatrix());
	m_Sphere.Update((D3DXMATRIX&)GetFrame().GetWorldMatrix());

	// update hgrid
	CHGrid::GetInstance()->RemoveObjectFromHGrid(this);
	CHGrid::GetInstance()->AddObjectToHGrid(this);
}

void CBase::Render()
{
}

bool CBase::CheckCollision(CBase* pBase)
{
	return m_pBV->Test(pBase->GetBV());
}

void CBase::CollisionReact(CBase* pBase)
{
}

bool CBase::CheckSceneCollision(CBase* pBase)
{
	if(GetType() == OBJ_TOPLASER || GetType() == OBJ_BOTTOMLASER || GetType() == OBJ_CONVEYOR || GetType() == OBJ_CANNON || GetType() == OBJ_ULTIMUS)
		return false;

	// ground clamp ray
	TAABB &a = m_AABB.GetAABB();
	//TAABB &a = m_BV.GetAABB();
	//TAABB &b = m_BV.GetPrevAABB();

	D3DXVECTOR3 curToPrev = m_vPrevPosition - m_vPosition;

	D3DXVECTOR3 rayStartOffset;
	D3DXVECTOR3 rayStart;
	D3DXVECTOR3 rayDir;

	rayStart = D3DXVECTOR3(a.center.x, a.maxPt.y, a.center.z);
	rayStartOffset = rayStart;
	rayStartOffset.y += curToPrev.y;
	float fRange = rayStartOffset.y - a.minPt.y;

	rayDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_bIsOnGround = false;
	m_bHitWall = false;
	bool bIgnore = false;
	switch(pBase->GetType())
	{
	case OBJ_GROUND:
		{
		}
		break;

	case OBJ_PLATFORM:
		{
			if(GetType() == OBJ_ROCKET_ALTERNATE)
				this->SetIsActive(false);

			if(m_vVelocity.y < 0.0f)
			{
				rayStart = D3DXVECTOR3(a.center.x, a.minPt.y, a.center.z);
				rayStartOffset = rayStart;
				rayStartOffset.y += curToPrev.y;
				fRange = rayStartOffset.y - a.minPt.y;
			}
			else if(m_vVelocity.y > 0.0f)
			{
				return false;
			}
		}
		break;

	case OBJ_WALL:
		{
			rayDir.y = 0.0f; rayDir.z = 0.0f;
			rayDir.x = m_vVelocity.x > 0.0f ?  1.0f : -1.0f;
			rayStart = D3DXVECTOR3(a.center.x, a.center.y, a.center.z);
			rayStartOffset = rayStart;
			rayStartOffset.x += curToPrev.x;
			fRange = rayStart.x - a.minPt.x;
			m_bHitWall = true;
		}
		break;
	}

	D3DXVECTOR3 vF;
	float fDist;

	if(CCollisionTests::TestRayMesh(rayStartOffset, rayDir, pBase->GetShape(0).GetMesh(),
		pBase->GetFrame().GetWorldMatrix(), vF, fDist, &m_faceNormal))
	{
		if(m_bHitWall)
		{
			if(fDist <= fRange )
			{
				m_vVelocity.x = 0.0f;
				CCollisionResponse::ResponseRayMesh(this, rayStartOffset, rayDir, vF, fDist, fRange);
			}
		}
		else
		{
			if(fDist <= (fRange + COLLISION_EPSILON))
			{
				m_bIsOnGround = true;
				m_vVelocity.y = 0.0f;
				CCollisionResponse::ResponseRayMesh(this, rayStartOffset, rayDir, vF, fDist, fRange);
			}
		}
	}

	return m_bIsOnGround || m_bHitWall;
}

void CBase::SceneCollisionReact(CBase* pBase)
{
}

void CBase::InitShape(CModel *model, CRenderContext* renderContext, D3DXMATRIX &mxWorld,
					  void (*RenderFunc)(CRenderNode&), bool bIsAnimated )
{
	AddShape(model, renderContext, mxWorld, RenderFunc, bIsAnimated);

	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);
	m_BVShape.GetFrame().GetLocalMatrix() = ident;
	m_BVShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetWireframeContext());
	m_BVShape.RenderFunc = CRenderShape::PCRenderFunc;

	m_AABB.Clone(&model->GetMesh().GetAABB());
	m_Sphere.Clone(&model->GetMesh().GetSphere());

	m_vPosition = D3DXVECTOR3(mxWorld._41, mxWorld._42, mxWorld._43);
	m_AABB.Update(mxWorld);
	m_Sphere.Update(mxWorld);
}

void CBase::AddShape(CModel* model, CRenderContext* renderContext, D3DXMATRIX &mxWorld,
					 void (*RenderFunc)(CRenderNode&), bool bIsAnimated )
{
	CRenderShape newShape;
	newShape.Init(&model->GetMesh(), bIsAnimated ? model : NULL, renderContext, mxWorld, RenderFunc);
	if(bIsAnimated) newShape.GetAnimCtrl().update(0.0f);
	m_Shapes.push_back(newShape);

	m_bHasAnimation = bIsAnimated;
}

void CBase::AddToRenderSet()
{
	for(size_t i = 0; i < m_Shapes.size(); i++)
		m_Shapes[i].GetRenderContext()->AddRenderNode(&m_Shapes[i]);
}

void CBase::AddToRenderSet(CRenderSet &renderSet)
{
	for(size_t index = 0; index < m_Shapes.size(); ++index)
		renderSet.AddRenderNode(&m_Shapes[index]);
}

void CBase::AddToBVRenderSet()
{
	m_BVShape.GetRenderContext()->AddRenderNode(&m_BVShape);
}

void CBase::SetZDepth(float fDepth)
{
	for(size_t i = 0; i < m_Shapes.size(); i++)
		m_Shapes[i].SetZDepth(fDepth);
}

void CBase::SetRenderContext(CRenderContext &context)
{
	for(size_t index = 0; index < m_Shapes.size(); ++index)
		m_Shapes[index].SetRenderContext(&context);
}

void CBase::SetRenderFunc(void (*RenderFunc)(CRenderNode&))
{
	for(size_t index = 0; index < m_Shapes.size(); ++index)
		m_Shapes[index].RenderFunc = RenderFunc;
}