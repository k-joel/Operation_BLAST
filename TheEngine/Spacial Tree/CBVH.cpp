//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CBVH.cpp"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: A bounding volume heirarchy used to traverse the renderable and collisions	//
//			and perform frustrum culling.			 									//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#include "CBVH.h"
#include <assert.h>		// code in assertions gets compiled out in Release mode
#include <fstream>

#include "../Collision/CCollisionTests.h"
#include "../Camera/Camera.h"
#include "../Rendering/CRenderShape.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Game Objects/CObjectManager.h"

CBVH CBVH::m_Instance;

CBVH *CBVH::GetInstance(void)
{
	return &m_Instance;
}

void CBVH::Build(vector<CBase*>* pRenderObjs,
		   vector<CBase*>* pCollObjs)
{
	m_pRootRenderNode = BuildBVHTree(pRenderObjs, pRenderObjs->size());
	m_pRootCollisionNode = BuildBVHTree(pCollObjs, pCollObjs->size());
}

PBVH_Node CBVH::BuildBVHTree(vector<CBase*>* pObjects, int nObjectCount)
{
	assert(nObjectCount != 0);
	int i, j;

	// Allocate temporary memory for holding node pointers to
	// the current set of active nodes (initially the leaves)
	PBVH_Node *pNodes = new PBVH_Node[nObjectCount];
	// Form the leaf nodes for the given input objects
	for (i = 0; i < nObjectCount; i++)
	{
		pNodes[i] = new BVH_Node;
		pNodes[i]->m_nType = LEAF;
		//pNodes[i]->m_pObject = (*pObjects)[i];
		pNodes[i]->m_nObjectID = i;
		pNodes[i]->m_pLeft = NULL;
		pNodes[i]->m_pRight = NULL;

		CMesh *pMesh = (*pObjects)[i]->GetShape(0).GetMesh();

		void *pVerts;
		pMesh->GetVertexBuffer()->Lock(0,0, &pVerts, D3DLOCK_READONLY);

		VertexPNT *newVerts = new VertexPNT[pMesh->GetVertexCount()];
		D3DXVec3TransformCoordArray(&newVerts[0].pos, pMesh->GetVertexStride(), (D3DXVECTOR3*)pVerts, pMesh->GetVertexStride(),
			&(*pObjects)[i]->GetFrame().GetWorldMatrix(), pMesh->GetVertexCount());

		pNodes[i]->m_AABB.CreateFromVerts(&newVerts[0].pos, pMesh->GetVertexCount(), pMesh->GetVertexStride());
		pNodes[i]->m_Sphere.CreateFromVerts(&newVerts[0].pos, pMesh->GetVertexCount(), pMesh->GetVertexStride());

		delete newVerts;
		pMesh->GetVertexBuffer()->Unlock();
	}

	// Merge pairs together until just the root object left
	while (nObjectCount > 1)
	{
		// Find indices of the two "nearest" nodes, based on some criterion
		FindNodesToMerge(pNodes, nObjectCount, i, j);
		// Group nodes i and j together under a new internal node
		PBVH_Node pPair = new BVH_Node;
		pPair->m_nType = NODE;
		pPair->m_pLeft = pNodes[i];
		pPair->m_pRight = pNodes[j];

		// Compute a bounding volume for the two nodes
		MergeNodeBV(pPair, pNodes[i], pNodes[j]);

		// Remove the two nodes from the active set and add in the new node.
		// Done by putting new node at index ’min’ and copying last entry to ’max’
		int min = i, max = j;
		if (i > j) min = j, max = i;
		pNodes[min] = pPair;
		pNodes[max] = pNodes[nObjectCount - 1];
		nObjectCount--;
	}

	// Free temporary storage and return root of tree
	PBVH_Node pRoot = pNodes[0];
	delete pNodes;
	return pRoot;
}

void CBVH::FindNodesToMerge(PBVH_Node *pNodes, int nObjectCount, int &nObj1, int &nObj2)
{
	float lowestdist = FLT_MAX;
	for(int i = 0; i < nObjectCount; i++)
	{
		for(int j = i+1; j < nObjectCount; j++)
		{
			D3DXVECTOR3 v = pNodes[j]->m_Sphere.GetSphere().c - pNodes[i]->m_Sphere.GetSphere().c;
			float sqdist = D3DXVec3LengthSq(&v);
			if(abs(sqdist) < lowestdist)
			{
				lowestdist = abs(sqdist);
				nObj1 = i;
				nObj2 = j;
			}
		}
	}
}

void CBVH::MergeNodeBV(PBVH_Node retNode, PBVH_Node a, PBVH_Node b)
{
	TSphere sphere;
	TAABB aabb;

	CCollisionTests::MergeSphereSphere(sphere, a->m_Sphere.GetSphere(), b->m_Sphere.GetSphere());
	CCollisionTests::MergeAABBAABB(aabb, a->m_AABB.GetAABB(), b->m_AABB.GetAABB());

	retNode->m_Sphere.GetSphere() = sphere;
	retNode->m_AABB.GetAABB() = aabb;
}

void CBVH::TraverseCollisions(PBVH_Node pNode, CBase* pObj, vector<CBase*>* resultVec )
{
	if (CCollisionTests::TestAABBAABB(pNode->m_AABB.GetAABB(), pObj->GetBVAABB().GetAABB()))
	{
		if (pNode->m_nType == NODE)
		{
			TraverseCollisions(pNode->m_pLeft, pObj, resultVec);
			TraverseCollisions(pNode->m_pRight, pObj, resultVec);

			return;
		}

		//pNode->shape.GetRenderContext()->AddRenderNode(&pNode->shape);
		(*resultVec).push_back(CObjectManager::GetInstance()->m_vSceneCollisionList[pNode->m_nObjectID]);
	}
}

void CBVH::TraverseDraw(PBVH_Node pNode)
{
	if (CCollisionTests::TestFrustrumSphere2(pNode->m_Sphere.GetSphere(), CCamera::GetInstance()->GetFrustum()))
	{
		if (pNode->m_nType == NODE)
		{
			TraverseDraw(pNode->m_pLeft);
			TraverseDraw(pNode->m_pRight);
			return;
		}

		//pNode->shape.GetRenderContext()->AddRenderNode(&pNode->shape);
		CObjectManager::GetInstance()->RenderAnObject(CObjectManager::GetInstance()->m_vSceneRenderableList[pNode->m_nObjectID]);
	}
}

void CBVH::TraverseDelete(PBVH_Node pNode)
{
	if (pNode->m_nType == NODE)
	{
		TraverseDelete(pNode->m_pLeft);
		TraverseDelete(pNode->m_pRight);
	}

	delete pNode;
}

void CBVH::ShutdownBVH()
{
	if(m_pRootRenderNode) { TraverseDelete(m_pRootRenderNode); m_pRootRenderNode = NULL; }
	if(m_pRootCollisionNode) { TraverseDelete(m_pRootCollisionNode); m_pRootCollisionNode = NULL; }
}

void CBVH::WriteBVH()
{
	fstream fout;
	fout.open("renderTree.NNbvh", ios_base::out | ios_base::binary | ios_base::trunc);

	if(fout.is_open())
	{
		int nCount = 0;
		TraverseWrite(m_pRootRenderNode, nCount, fout);
		fout.write((char*)&nCount, 4);

		fout.close();
		fout.clear();
	}

	fout.open("collisionTree.NNbvh", ios_base::out | ios_base::binary | ios_base::trunc);

	if(fout.is_open())
	{
		int nCount = 0;
		TraverseWrite(m_pRootCollisionNode, nCount, fout);
		fout.write((char*)&nCount, 4);

		fout.close();
		fout.clear();
	}
}

void CBVH::TraverseWrite(PBVH_Node pNode, int &nCount, fstream &f)
{
	int nLeftIdx, nRightIdx;

	if (pNode->m_nType == NODE)
	{
		TraverseWrite(pNode->m_pLeft, nCount, f);
		nLeftIdx = nCount-1;

		TraverseWrite(pNode->m_pRight, nCount, f);
		nRightIdx = nCount-1;
	}

	f.write((char*)pNode, sizeof(BVH_Node));

	if (pNode->m_nType == NODE)
	{
		f.write((char*)&nLeftIdx, 4);
		f.write((char*)&nRightIdx, 4);
	}

	nCount++;
}

PBVH_Node CBVH::ReadFile(fstream &f)
{
	f.seekg(0, ios_base::end);
	unsigned int nFileLength = (unsigned int)f.tellg();

	fstream::pos_type pos = f.tellg();
	pos -= 4; // sub the count size and read it in
	f.seekg(pos);
	int nNodeCount;
	f.read((char*)&nNodeCount, 4);

	f.seekg(0, ios_base::beg);
	//nFileLength -= 2; // sub the eof info from the filelength

	/*unsigned int size = sizeof(BVH_Node);
	unsigned int nNodeCount = nFileLength/size;
	float div = (float)nFileLength / (float)size;*/
	PBVH_Node *pNodes = new PBVH_Node[nNodeCount];

	for(int i = 0; i < nNodeCount; i++)
	{
		pNodes[i] = new BVH_Node;
		f.read((char*)pNodes[i], sizeof(BVH_Node));

		if(pNodes[i]->m_nType == NODE)
		{
			int nLeftIdx, nRightIdx;
			f.read((char*)&nLeftIdx, 4);
			f.read((char*)&nRightIdx, 4);

			pNodes[i]->m_pLeft = pNodes[nLeftIdx];
			pNodes[i]->m_pRight = pNodes[nRightIdx];
		}
	}

	// Free temporary storage and return root of tree
	PBVH_Node pRoot = pNodes[nNodeCount-1];
	delete pNodes;
	return pRoot;
}

void CBVH::ReadBVH()
{
	fstream fin;
	fin.open("renderTree.NNbvh", ios_base::in | ios_base::binary);

	if(fin.is_open())
	{
		m_pRootRenderNode = ReadFile(fin);

		fin.close();
		fin.clear();
	}

	fin.open("collisionTree.NNbvh", ios_base::in | ios_base::binary);

	if(fin.is_open())
	{
		m_pRootCollisionNode = ReadFile(fin);

		fin.close();
		fin.clear();
	}
}