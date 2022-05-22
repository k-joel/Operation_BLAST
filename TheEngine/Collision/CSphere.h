#pragma once
#include "CBaseBV.h"

class CSphere : public CBaseBV
{
private:
	TSphere m_Sphere;
	TSphere m_OriginalSphere;

public:
	CSphere() { m_nType = BV_SPHERE; }
	~CSphere() {}

	void CreateFromVerts(D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize);
	void Clone( CBaseBV *pBV );
	void Update(D3DXMATRIX &mat);
	void CreateMesh(CBaseBV *pBV);
	bool Test(CBaseBV *pBV);

	TSphere &GetSphere() { return m_Sphere; }
	TSphere &GetOriginalSphere() { return m_OriginalSphere; }
};