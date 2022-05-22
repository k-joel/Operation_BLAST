//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CAABB.cpp"																	//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose:	Handles AABB creation and collision with other primitive types				//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
#include "CAABB.h"
#include "CSphere.h"
#include "COBB.h"
#include "CCapsule.h"
#include "../Mesh/CMesh.h"

//////////////////////////////////////////////////////////////////////////////////////
//	Function:	"CreateFromVerts"													//
//																					//
//	Input:		pVerts		|	The array of vertices to create from				//
//				dwNumVerts	|	The number of vertices in the array					//
//				dwSize		|	The stride offset of each vertex					//
//																					//
//	Return:		void		|	Nothing												//
//																					//
//	Purpose:	Creates an AABB from the vertices passed in							//
//////////////////////////////////////////////////////////////////////////////////////
void CAABB::CreateFromVerts( D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize )
{
	D3DXComputeBoundingBox(pVerts, dwNumVerts, dwSize, &m_AABB.minPt, &m_AABB.maxPt);

	m_AABB.center = (m_AABB.maxPt + m_AABB.minPt) * 0.5f;

	memcpy(&m_OriginalAABB, &m_AABB, sizeof(TAABB));
}

//////////////////////////////////////////////////////////////////////////////////////
//	Function:	"Clone"																//
//																					//
//	Input:		pBV		|	The bounding volume to duplicate 						//
//																					//
//	Return:		void	|	Nothing													//
//																					//
//	Purpose:	Duplicates an AABB from the one passed in							//
//////////////////////////////////////////////////////////////////////////////////////
void CAABB::Clone( CBaseBV *pBV )
{
	if(pBV->GetType() != BV_AABB) return;

	TAABB &aabb = ((CAABB*)pBV)->GetAABB();

	memcpy(&m_AABB, &aabb, sizeof(TAABB));
	memcpy(&m_OriginalAABB, &aabb, sizeof(TAABB));
}

//////////////////////////////////////////////////////////////////////////////////////
//	Function:	"Update"															//
//																					//
//	Input:		mat		|	The transformed matrix 									//
//																					//
//	Return:		void	|	Nothing													//
//																					//
//	Purpose:	Transforms the AABB by the matrix passed in							//
//////////////////////////////////////////////////////////////////////////////////////
void CAABB::Update( D3DXMATRIX &mat )
{
	TAABB &a = m_OriginalAABB;
	TAABB &b = m_AABB;

	D3DXVECTOR3 pos = D3DXVECTOR3(mat._41, mat._42, mat._43);

	// For all three axes
	for (int i = 0; i < 3; i++)
	{
		// Start by adding in translation
		b.minPt[i] = b.maxPt[i] = pos[i];
		// Form extent by summing smaller and larger terms respectively
		for (int j = 0; j < 3; j++)
		{
			float e = mat(i, j) * a.minPt[j];
			float f = mat(i, j) * a.maxPt[j];
			if (e < f) {
				b.minPt[i] += e;
				b.maxPt[i] += f;
			} else {
				b.minPt[i] += f;
				b.maxPt[i] += e;
			}
		}
	}

	m_AABB.center = (m_AABB.minPt + m_AABB.maxPt) * 0.5f;
}

//////////////////////////////////////////////////////////////////////////////////////
//	Function:	"CreateMesh"														//
//																					//
//	Input:		pBV		|	The AABB to create from									//
//																					//
//	Return:		void	|	Nothing													//
//																					//
//	Purpose:	Creates and stores a bounding mesh from the passed in AABB			//
//////////////////////////////////////////////////////////////////////////////////////
void CAABB::CreateMesh( CBaseBV *pBV )
{
	if(pBV->GetType() != BV_AABB) return;

	TAABB &tAABB = ((CAABB*)pBV)->GetAABB();

	// indexed cube
	VertexPC sqverts[8];
	sqverts[0].pos = D3DXVECTOR3( tAABB.minPt.x, tAABB.minPt.y, tAABB.minPt.z);	// left bottom front
	sqverts[1].pos = D3DXVECTOR3( tAABB.minPt.x, tAABB.maxPt.y, tAABB.minPt.z);	// left top front
	sqverts[2].pos = D3DXVECTOR3( tAABB.maxPt.x, tAABB.maxPt.y, tAABB.minPt.z);	// right top front
	sqverts[3].pos = D3DXVECTOR3( tAABB.maxPt.x, tAABB.minPt.y, tAABB.minPt.z);	// right bottom front

	sqverts[4].pos = D3DXVECTOR3( tAABB.minPt.x, tAABB.minPt.y, tAABB.maxPt.z);	// left bottom back
	sqverts[5].pos = D3DXVECTOR3( tAABB.minPt.x, tAABB.maxPt.y, tAABB.maxPt.z);	// left top back
	sqverts[6].pos = D3DXVECTOR3( tAABB.maxPt.x, tAABB.maxPt.y, tAABB.maxPt.z);	// right top back
	sqverts[7].pos = D3DXVECTOR3( tAABB.maxPt.x, tAABB.minPt.y, tAABB.maxPt.z);	// right bottom back

	for(int i = 0; i < 8; i++)
		sqverts[i].col = WHITE;

	WORD indices[36];

	//front face
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	//back face
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	//left face
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	//right face
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	//top face
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	//bottom face
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	// Create mesh
	SAFE_DELETE(m_pMesh);
	m_pMesh = new CMesh();
	m_pMesh->BuildMesh(D3DPT_TRIANGLELIST, 12, (void*)sqverts, 8, sizeof(VertexPC),
		(void*)indices, 36, sizeof(WORD));
}

//////////////////////////////////////////////////////////////////////////////////////
//	Function:	"CreateMesh															//
//																					//
//	Input:		pBV		|	The collision primitive to test against					//
//																					//
//	Return:		bool	|	The result of the collision								//
//																					//
//	Purpose:	Performs collision against other types of primitives				//
//////////////////////////////////////////////////////////////////////////////////////
bool CAABB::Test( CBaseBV *pBV )
{
	switch(pBV->GetType())
	{
	case BV_AABB:
		{
			CAABB *pAABB = (CAABB*)pBV;
			return CCollisionTests::TestAABBAABB(m_AABB, pAABB->GetAABB());
		}
		break;
	case BV_SPHERE:
		{
			CSphere *pSphere = (CSphere*)pBV;
			return CCollisionTests::TestSphereAABB(pSphere->GetSphere(), m_AABB);
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
			return CCollisionTests::TestCapsuleAABB(pCapsule->GetCapsule(), m_AABB);
		}
		break;
	}

	return false;
}