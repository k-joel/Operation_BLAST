#include "CBoxTest.h"
#include "..\..\Input\CInput.h"
#include "..\..\States\CGameState.h"
#include "..\..\Rendering\CRenderer.h"
#include "..\..\Rendering\CShaderManager.h"

CBoxTest::CBoxTest()
{
	m_BoxMesh = NULL;
}

CBoxTest::~CBoxTest()
{
	SAFE_RELEASE(m_BoxMesh);
}

void CBoxTest::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CBoxTest::Render()
{
	TMaterial x;
	x.m_Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	x.m_Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

bool CBoxTest::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CBoxTest::CollisionReact(CBase* pBase)
{
	CBase::CollisionReact(pBase);
}