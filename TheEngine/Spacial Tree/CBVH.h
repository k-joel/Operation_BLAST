//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CBVH.h"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: A bounding volume heirarchy used to traverse the renderable and collisions	//
//			and perform frustrum culling.			 									//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../Game Objects/CBase.h"
#include "../Collision/CAABB.h"
#include "../Collision/CSphere.h"

enum NodeTypes { LEAF, NODE };

typedef struct _BVH_Node
{
	int m_nType;
	int m_nObjectID;
	_BVH_Node* m_pLeft;
	_BVH_Node* m_pRight;
	CAABB m_AABB;
	CSphere m_Sphere;
} BVH_Node, *PBVH_Node;

class CBVH
{
	friend class CObjectManager;

private:
	PBVH_Node m_pRootCollisionNode;
	PBVH_Node m_pRootRenderNode;

	int m_nCollisionNodeCount;
	int m_nRenderNodeCount;

	CBVH() : m_pRootCollisionNode(0), m_pRootRenderNode(0) {}
	~CBVH() {}
	CBVH(const CBVH&);
	CBVH& operator=(const CBVH&);

	static CBVH		m_Instance;			// An instance to this class.

	PBVH_Node BuildBVHTree(vector<CBase*>* pObjects, int nObjectCount);
	void FindNodesToMerge(PBVH_Node *pNodes, int nObjectCount, int &nObj1, int &nObj2);
	void MergeNodeBV(PBVH_Node retNode, PBVH_Node a, PBVH_Node b);

public:
	static CBVH *GetInstance(void);

	PBVH_Node GetRootCollisionNode() { return m_pRootCollisionNode; }
	PBVH_Node GetRootRenderNode() { return m_pRootRenderNode; }

	void Build(vector<CBase*>* pRenderObjs,
		vector<CBase*>* pCollObjs);

	// TODO
	void WriteBVH();
	void ReadBVH();

	void TraverseWrite(PBVH_Node pNode, int &nCount, fstream &f);
	PBVH_Node ReadFile(fstream &f);

	//bool TraverseCollisions(PBVHNode pNode, CBase* pObj, vector<CBase*>* resultVec);
	void TraverseCollisions(PBVH_Node pNode, CBase* pObj, vector<CBase*>* resultVec);

	void TraverseDraw(PBVH_Node pNode);

	void TraverseDelete(PBVH_Node pNode);
	void ShutdownBVH();
};