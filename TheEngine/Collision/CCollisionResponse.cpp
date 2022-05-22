#include "CCollisionResponse.h"
#include "../Game Objects/CBase.h"

void CCollisionResponse::ResponseRayMesh(CBase* pObj, D3DXVECTOR3 &rayStart, D3DXVECTOR3 &rayDir, D3DXVECTOR3 &vF, float fDist, float fRange)
{
	D3DXVECTOR3 rayVec = rayDir * fRange;
	D3DXVECTOR3 projPt = rayStart + rayVec;
	D3DXVECTOR3 dispVec = vF - projPt;
	pObj->m_vPosition += dispVec;

	D3DXVECTOR3 epsilonVec = rayDir * 0.1f;
	pObj->m_vPosition += epsilonVec;

	/*D3DXVECTOR3 rayInv(-rayDir.x, -rayDir.y, -rayDir.z);

	rayInv *= (fRange - fDist);
	pObj->m_vPosition += rayInv;*/

	/*float height =  pObj->m_BV.GetAABB().center.y - pObj->m_BV.GetAABB().minPt.y;

	rayDir *= fDist - height;
	pObj->m_vPosition += rayDir;
	pObj->m_vVelocity.y = 0.0f;*/
}

void CCollisionResponse::ResponseSegmentMesh(CBase* pObj, D3DXVECTOR3 &p, D3DXVECTOR3 &q, float fU, float fV, float fW)
{
}

void CCollisionResponse::ResponseSphereMesh(CBase* pObj, TSphere &s, float fU, float fV, float fW, float fDist)
{
}