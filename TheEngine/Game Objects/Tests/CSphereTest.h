#pragma once

#include "..\CBase.h"

class CSphereTest : public CBase
{
	LPD3DXMESH m_SphereMesh;

public:
	CSphereTest();
	~CSphereTest();

	void SetMesh(LPD3DXMESH mesh) { m_SphereMesh = mesh; }

	void Update(float fElapsedTime);
	void Render();
	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};