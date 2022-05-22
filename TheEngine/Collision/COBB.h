#pragma once
#include "CBaseBV.h"

class COBB : public CBaseBV
{
private:
	TOBB m_OBB;
	TOBB m_OriginalOBB;

public:
	COBB() { m_nType = BV_OBB; }
	~COBB() {}

	void CreateFromVerts(D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize);
	void Clone( CBaseBV *pBV );
	void Update(D3DXMATRIX &mat);
	void CreateMesh(CBaseBV *pBV);
	bool Test(CBaseBV *pBV);

	TOBB &GetOBB() { return m_OBB; }
};