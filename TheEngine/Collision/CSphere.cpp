#include "CSphere.h"
#include "CCapsule.h"
#include "COBB.h"
#include "CAABB.h"
#include "../Mesh/CMesh.h"

// constants to build the sphere mesh
const int nStacks = 8;
const int nSlices = 8;

void CSphere::CreateFromVerts( D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize )
{
	D3DXComputeBoundingSphere(pVerts, dwNumVerts, dwSize, &m_Sphere.c, &m_Sphere.r );

	memcpy(&m_OriginalSphere, &m_Sphere, sizeof(TSphere));
}

void CSphere::Clone( CBaseBV *pBV )
{
	if(pBV->GetType() != BV_SPHERE) return;

	TSphere &sphere = ((CSphere*)pBV)->GetSphere();

	memcpy(&m_Sphere, &sphere, sizeof(TSphere));
	memcpy(&m_OriginalSphere, &sphere, sizeof(TSphere));
}

void CSphere::Update( D3DXMATRIX &mat )
{
	D3DXVec3TransformCoord(&m_Sphere.c, &m_OriginalSphere.c, &mat);
	//m_Sphere.c = m_OriginalSphere.c + pos;
}

void CSphere::CreateMesh( CBaseBV *pBV )
{
	if(pBV->GetType() != BV_SPHERE) return;

	TSphere &sphere = ((CSphere*)pBV)->GetSphere();

	//calculates the resulting number of vertices and indices
	int nVertices = (nStacks + 1) * (nSlices + 1);
	int nIndices = (3 * nStacks * (nSlices + 1)) * 2;

	VertexPC *sphereVerts = new VertexPC[nVertices];
	WORD *sphereIndices = new WORD[nIndices];

	float fStackAngle = D3DX_PI / (float)nStacks;
	float fSliceAngle = (float)(D3DX_PI * 2.0f) / (float)nSlices;

	int nVertexIndex = 0;

	//Generate the group of Stacks for the sphere
	int nVertCount = 0;
	int nIndexCount =0;

	for (int stack = 0; stack < (nStacks+1); stack++)
	{
		float r = (float)sin((float)stack * fStackAngle);
		float y = (float)cos((float)stack * fStackAngle);

		//Generate the group of segments for the current Stack
		for (int slice = 0; slice < (nSlices+1); slice++)
		{
			float x = r * (float)sin((float)slice * fSliceAngle);
			float z = r * (float)cos((float)slice * fSliceAngle);
			sphereVerts[nVertCount].pos = D3DXVECTOR3(x * sphere.r, y * sphere.r, z * sphere.r); //normalized
			sphereVerts[nVertCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			nVertCount++;
			if (!(stack == (nStacks - 1)))
			{
				sphereIndices[nIndexCount++] = nVertexIndex + (nSlices + 1);
				sphereIndices[nIndexCount++] = nVertexIndex + 1;
				sphereIndices[nIndexCount++] = nVertexIndex;
				sphereIndices[nIndexCount++] = nVertexIndex + (nSlices);
				sphereIndices[nIndexCount++] = nVertexIndex + (nSlices + 1);
				sphereIndices[nIndexCount++] = nVertexIndex;
				nVertexIndex++;
			}
		}
	}

	// square
	SAFE_DELETE(m_pMesh);
	m_pMesh = new CMesh();
	m_pMesh->BuildMesh(D3DPT_TRIANGLELIST, nIndices/3, (void*)sphereVerts, nVertices, sizeof(VertexPC),
		(void*)sphereIndices, nIndices, sizeof(WORD));

	delete[] sphereVerts;
	delete[] sphereIndices;
}

bool CSphere::Test( CBaseBV *pBV )
{
	switch(pBV->GetType())
	{
	case BV_AABB:
		{
			CAABB *pAABB = (CAABB*)pBV;
			return CCollisionTests::TestSphereAABB(m_Sphere, pAABB->GetAABB());
		}
		break;
	case BV_SPHERE:
		{
			CSphere *pSphere = (CSphere*)pBV;
			return CCollisionTests::TestSphereSphere(m_Sphere, pSphere->GetSphere());
		}
		break;
	case BV_OBB:
		{
			COBB *pOBB = (COBB*)pBV;
			D3DXVECTOR3 p;
			return CCollisionTests::TestSphereOBB(m_Sphere, pOBB->GetOBB(), p);
		}
		break;
	case BV_CAPSULE:
		{
			CCapsule *pCapsule = (CCapsule*)pBV;
			return CCollisionTests::TestSphereCapsule(m_Sphere, pCapsule->GetCapsule());
		}
		break;
	}

	return false;
}