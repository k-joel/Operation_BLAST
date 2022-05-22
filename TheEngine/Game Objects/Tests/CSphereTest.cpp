#include "CSphereTest.h"
#include "..\..\Collision\CCollisionTests.h"

CSphereTest::CSphereTest()
{
	m_SphereMesh = NULL;
}

CSphereTest::~CSphereTest()
{
	SAFE_RELEASE(m_SphereMesh);
}

void CSphereTest::Update(float fElapsedTime)
{
	//Input(fElapsedTime);
}

void CSphereTest::Render()
{

}

bool CSphereTest::CheckCollision(CBase* pBase)
{
	return true;
}

void CSphereTest::CollisionReact(CBase* pBase)
{

}