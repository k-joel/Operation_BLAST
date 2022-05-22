#include "CFizzle.h"
#include "../../Mesh/CModel.h"
#include "../../Mesh/CModelManager.h"
#include "../../Rendering/CRenderContextManager.h"
#include "../CObjectFactory.h"
#include "../CObjectManager.h"

CFizzle::CFizzle()
{
	SetType(OBJ_FIZZLE);
	SetIsAffectedByGrav(false);
	SetRotationY(D3DX_PI * 0.5);


}
void CFizzle::Initialize()
{
	SetIsActive(true);
	SetRotationX(0);

	VertexPNT *pVerts;
	this->GetShape(0).GetMesh()->GetVertexBuffer()->Lock(0,0, (void**)&pVerts, D3DLOCK_READONLY);
	m_Capsule.CreateFromVerts((D3DXVECTOR3*)pVerts,GetShape(0).GetMesh()->GetVertexCount(), GetShape(0).GetMesh()->GetVertexStride());
	this->GetShape(0).GetMesh()->GetVertexBuffer()->Unlock();
}

CFizzle::~CFizzle()
{
}

void CFizzle::Update(float fDt)
{

	static float m_fDespawnTimer = 0.0f;
	m_fDespawnTimer += fDt;
	m_Capsule.Update((D3DXMATRIX&)GetFrame().GetWorldMatrix());

	if(m_fDespawnTimer > 3.0f)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}
	if(m_fDespawnTimer > 0.47f)
		SetVelocity(0,0,0);

	D3DXVECTOR3 nDir, worldY, newX,newY;
	nDir = D3DXVECTOR3(GetFrame().GetLocalMatrix()._31,GetFrame().GetLocalMatrix()._32,GetFrame().GetLocalMatrix()._33);
	CBase::Update(fDt);

	if(!FLOATEQUALS(GetVelocity().x,0.0f,0.01f) )
	{
		nDir = GetVelocity();
	}
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	worldY = D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVec3Normalize(&nDir, &nDir);

	D3DXVec3Cross(&newX, &nDir, &worldY);
	D3DXVec3Normalize(&newX, &newX);

	D3DXVec3Cross(&newY, &newX, &nDir);
	D3DXVec3Normalize(&newY, &newY);

	newOrien._11 = newX.x;
	newOrien._12 = newX.y;
	newOrien._13 = newX.z;

	newOrien._21 = newY.x;
	newOrien._22 = newY.y;
	newOrien._23 = newY.z;

	newOrien._31 = nDir.x;
	newOrien._32 = nDir.y;
	newOrien._33 = nDir.z;

	GetFrame().GetLocalMatrix() = newOrien;


}

void CFizzle::Render()
{
	CBase::Render();
}

bool CFizzle::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CFizzle::CollisionReact(CBase *pBase)
{
}


