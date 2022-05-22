#pragma once

#include "..\CBase.h"

class CBoxTest : public CBase
{
	LPD3DXMESH m_BoxMesh;

public:
	CBoxTest();
	~CBoxTest();

	void SetMesh(LPD3DXMESH mesh) { m_BoxMesh = mesh; }

	void Update(float fElapsedTime);
	void Render();
	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);

};