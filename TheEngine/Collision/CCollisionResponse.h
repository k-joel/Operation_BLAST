#pragma once

#include <d3dx9math.h>
#include "CollisionObject.h"

class CMesh;
class CBase;
class CCollisionResponse
{
public:
	CCollisionResponse();
	~CCollisionResponse();

	static void ResponseRayMesh(CBase* pObj, D3DXVECTOR3 &rayStart, D3DXVECTOR3 &rayDir, D3DXVECTOR3 &vF, float fDist, float fRange);
	static void ResponseSegmentMesh(CBase* pObj, D3DXVECTOR3 &p, D3DXVECTOR3 &q, float fU, float fV, float fW);
	static void ResponseSphereMesh(CBase* pObj, TSphere &s, float fU, float fV, float fW, float fDist);
};