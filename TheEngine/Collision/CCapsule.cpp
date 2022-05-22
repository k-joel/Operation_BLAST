#include "CCapsule.h"
#include "COBB.h"
#include "CAABB.h"
#include "CSphere.h"

void CCapsule::CreateFromVerts( D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize )
{
	TAABB aabb;
	D3DXComputeBoundingBox(pVerts, dwNumVerts, dwSize, &aabb.minPt, &aabb.maxPt);

	aabb.center = (aabb.maxPt + aabb.minPt) * 0.5f;

	// figure out what orientation to make the capsule
	D3DXVECTOR3 lengths(aabb.maxPt.x - aabb.minPt.x, aabb.maxPt.y - aabb.minPt.y, aabb.maxPt.z - aabb.minPt.z);
	D3DXVECTOR3 halfLengths( lengths.x * 0.5f, lengths.y *0.5f, lengths.z*0.5f);

	/*float rad = lengths.y * 0.5f;
	D3DXVECTOR3 centerA(aabb.minPt.x+rad, aabb.minPt.y+rad, aabb.minPt.z+(lengths.z*0.5f));
	D3DXVECTOR3 centerB(aabb.maxPt.x-rad, aabb.maxPt.y-rad, aabb.maxPt.z-(lengths.z*0.5f));*/

	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	float rad = 0.0f;

	if( lengths.x > lengths.y && lengths.x > lengths.z)
	{
		dir.x = 1.0f;
		if( lengths.y > lengths.z)
			rad	= halfLengths.y;
		else
			rad = halfLengths.z;
	}
	else if( lengths.y > lengths.x && lengths.y > lengths.z)
	{
		dir.y = 1.0f;
		if( lengths.x > lengths.z)
			rad	= halfLengths.x;
		else
			rad = halfLengths.z;
	}
	else
	{
		dir.z = 1.0f;
		if( lengths.x > lengths.y)
			rad	= halfLengths.x;
		else
			rad = halfLengths.y;
	}

	D3DXVECTOR3 finalLen(dir.x * halfLengths.x, dir.y * halfLengths.y, dir.z * halfLengths.z);
	D3DXVECTOR3 radOffset = dir * rad;

	m_Capsule.a = aabb.center + finalLen - radOffset;
	m_Capsule.b = aabb.center - finalLen + radOffset;
	m_Capsule.r = rad;

	memcpy(&m_OriginalCapsule, &m_Capsule, sizeof(TCapsule));
}

void CCapsule::Clone( CBaseBV *pBV )
{
	if(pBV->GetType() != BV_CAPSULE) return;

	TCapsule &capsule = ((CCapsule*)pBV)->GetCapsule();

	memcpy(&m_Capsule, &capsule, sizeof(TCapsule));
	memcpy(&m_OriginalCapsule, &capsule, sizeof(TCapsule));
}

void CCapsule::Update( D3DXMATRIX &mat )
{
	D3DXVec3TransformCoord(&m_Capsule.a, &m_OriginalCapsule.a, &mat);
	D3DXVec3TransformCoord(&m_Capsule.b, &m_OriginalCapsule.b, &mat);
}

void CCapsule::CreateMesh( CBaseBV *pBV )
{
}

bool CCapsule::Test( CBaseBV *pBV )
{
	switch(pBV->GetType())
	{
	case BV_AABB:
		{
			CAABB *pAABB = (CAABB*)pBV;
			return CCollisionTests::TestCapsuleAABB(m_Capsule, pAABB->GetAABB());
		}
		break;
	case BV_SPHERE:
		{
			CSphere *pSphere = (CSphere*)pBV;
			return CCollisionTests::TestSphereCapsule(pSphere->GetSphere(), m_Capsule);
		}
		break;
	case BV_OBB:
		{
			return false;
		}
		break;
	case BV_CAPSULE:
		{
			CCapsule *pCapsule = (CCapsule*)pBV;
			return CCollisionTests::TestCapsuleCapsule(m_Capsule, pCapsule->GetCapsule());
		}
		break;
	}

	return false;
}