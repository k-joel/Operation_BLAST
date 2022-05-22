//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CCollisionTests.h"															//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose:	A library of functions to perform various collision tests					//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <d3dx9math.h>
#include "CollisionObject.h"

enum Intersection { CT_OUT, CT_INTERSECT, CT_IN };

class CMesh;
class CCollisionTests
{
public:
	CCollisionTests();
	~CCollisionTests();

	//////////////////////////////////////////////////////////////////////////////////////
	//	Function:	"FunctionName"														//
	//																					//
	//	Input:		a	|	First AABB													//
	//				b	|	Second AABB													//
	//																					//
	//	Return:		void	|	The result of the test									//
	//																					//
	//	Purpose:	AABB to AABB collision test											//
	//////////////////////////////////////////////////////////////////////////////////////
	static bool TestAABBAABB(TAABB &a, TAABB &b);

	static bool TestAABBAABB(TAABB &a, TAABB &b, D3DXVECTOR3 &dispVec);
	static bool TestAABBPlane(TAABB &b, D3DXPLANE &p);
	static float SqDistPointAABB(D3DXVECTOR3 &p, TAABB &b);
	static bool TestSphereAABB(TSphere &s, TAABB &b);
	static bool TestSphereSphere(TSphere &a, TSphere &b);
	static bool TestSpherePlane(TSphere &s, D3DXPLANE &p);
	static bool TestRayMesh(D3DXVECTOR3 &start, D3DXVECTOR3 &dir, CMesh *pMesh,
		D3DXMATRIX mxWorld, D3DXVECTOR3 &vF, float &fDist, D3DXVECTOR3 *n = NULL);
	static bool IntersectMovingAABBAABB(TAABB &a, TAABB &b, D3DXVECTOR3 &va, D3DXVECTOR3 &vb,
		float &tfirst, float &tlast);
	static bool TestAABBMesh(TAABB &a, CMesh *pMesh, D3DXMATRIX mxWorld, float &fDist);
	static bool TestFrustrumAABB(TAABB &a, D3DXPLANE *f);
	static bool TestFrustrumSphere(TSphere &s, D3DXPLANE *f);
	static int IntersectFrustrumAABB(TAABB &a, D3DXPLANE *f);
	static int IntersectFrustrumSphere(TSphere &s, D3DXPLANE *f);
	static int IntersectFrustrumSphere2(TSphere &s, D3DXPLANE *f);
	static void MergeAABBAABB(TAABB &a, TAABB &a0, TAABB &a1);
	static void MergeSphereSphere(TSphere &s, TSphere &s0, TSphere &s1);
	static bool TestFrustrumAABB2(TAABB &a, D3DXPLANE *f);
	static bool TestFrustrumSphere2(TSphere &s, D3DXPLANE *f);
	static bool TestSegmentMesh(D3DXVECTOR3 &start, D3DXVECTOR3 &end, CMesh *pMesh,
		D3DXMATRIX mxWorld, D3DXVECTOR3 &vF, float &t);
	static bool IntersectSegmentTriangle(D3DXVECTOR3 p, D3DXVECTOR3 q, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c,
		float &u, float &v, float &w, float &t);
	static bool TestAABBMesh(TAABB &a, CMesh *pMesh, D3DXMATRIX mxWorld);
	static bool TestAABBTriangle(D3DXVECTOR3 &v0, D3DXVECTOR3 &v1, D3DXVECTOR3 &v2, TAABB b);
	static bool IntersectSegmentPlane(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXPLANE p, float &t, D3DXVECTOR3 &q);
	static bool TestOBBOBB(TOBB &a, TOBB &b);
	static void ClosestPtPointOBB(D3DXVECTOR3 p, TOBB b, D3DXVECTOR3 &q);
	static bool TestSphereOBB(TSphere s, TOBB b, D3DXVECTOR3 &p);
	static bool TestAABBOBB(TAABB &a, TOBB &b);
	static float Clamp(float n, float min, float max);
	static float SqDistPointSegment(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);
	static float ClosestPtSegmentSegment(D3DXVECTOR3 p1, D3DXVECTOR3 q1, D3DXVECTOR3 p2, D3DXVECTOR3 q2,
		float &s, float &t, D3DXVECTOR3 &c1, D3DXVECTOR3 &c2);
	static bool TestSphereCapsule(TSphere &s, TCapsule &capsule);
	static bool TestCapsuleCapsule(TCapsule &capsule1, TCapsule &capsule2);
	static bool TestSegmentAABB(D3DXVECTOR3 &p0, D3DXVECTOR3 &p1, TAABB &b);
	static bool TestCapsuleAABB(TCapsule &c, TAABB &b);
};