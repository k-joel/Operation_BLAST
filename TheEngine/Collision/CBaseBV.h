#pragma once
#include "CCollisionTests.h"

enum eBVTypes { BV_BASE, BV_AABB, BV_SPHERE, BV_OBB, BV_CAPSULE };

class CBaseBV
{
protected:
	int m_nType;

	CMesh *m_pMesh;

public:
	CBaseBV();
	virtual ~CBaseBV();

	virtual void CreateFromVerts(D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize) {}
	virtual void Clone( CBaseBV *pBV ) {}
	virtual void Update(D3DXMATRIX &mat) {}
	virtual void CreateMesh(CBaseBV *pBV) {}
	virtual bool Test(CBaseBV *pBV) { return false; }

	CMesh *GetMesh() { return m_pMesh; }
	int GetType() { return m_nType; }
};