#pragma once
#include "CBaseBV.h"

class CCapsule : public CBaseBV
{
private:
	TCapsule m_Capsule;
	TCapsule m_OriginalCapsule;

	void ConstructCapsule() {}
public:
	CCapsule() { m_nType = BV_CAPSULE; }
	~CCapsule() {}

	void CreateFromVerts(D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize);
	void Clone( CBaseBV *pBV );
	void Update(D3DXMATRIX &mat);
	void CreateMesh(CBaseBV *pBV);
	bool Test(CBaseBV *pBV);

	TCapsule &GetCapsule() { return m_Capsule; }
};