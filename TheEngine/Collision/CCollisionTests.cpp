#include "CCollisionTests.h"
#include "CollisionObject.h"
#include "../Rendering/CDirect3D.h"
#include "../Debug System/DebugSystem.h"
#include "../Mesh/CMesh.h"

#define MIN3(a,b,c) min(a,min(b,c))
#define MAX3(a,b,c) max(a,max(b,c))

bool CCollisionTests::TestAABBAABB(TAABB &a, TAABB &b)
{
	if( a.maxPt.x < b.minPt.x || a.minPt.x > b.maxPt.x ) return false;
	if( a.maxPt.y < b.minPt.y || a.minPt.y > b.maxPt.y ) return false;
	if( a.maxPt.z < b.minPt.z || a.minPt.z > b.maxPt.z ) return false;

	return true;
}

bool CCollisionTests::TestAABBAABB(TAABB &a, TAABB &b, D3DXVECTOR3 &dispVec)
{
	bool bCollided = false;
	for(int i=0; i < 3; i++)
	{
		if( a.maxPt[i] > b.minPt[i] ) {
			dispVec[i] = a.minPt[i] - b.maxPt[i]; bCollided = true; }
		else if( a.minPt[i] < b.maxPt[i] ) {
			dispVec[i] = a.maxPt[i] - b.minPt[i]; bCollided = true; }
	}
	return bCollided;
}

bool CCollisionTests::TestAABBPlane(TAABB &b, D3DXPLANE &p)
{
	D3DXVECTOR3 c = (b.maxPt + b.minPt) * 0.5f;
	D3DXVECTOR3 e = b.maxPt - c;

	float r = e.x*abs(p.a) + e.y*abs(p.b) + e.z*abs(p.c);

	float s = D3DXPlaneDotCoord(&p, &c);
	return abs(s) <= r;
}

float CCollisionTests::SqDistPointAABB(D3DXVECTOR3 &p, TAABB &b)
{
	float sqDist = 0.0f;
	for(int i = 0; i < 3; i++)
	{
		float v = p[i];
		if(v < b.minPt[i]) sqDist += (b.minPt[i] - v) * (b.minPt[i] - v);
		if(v > b.maxPt[i]) sqDist += (v - b.maxPt[i]) * (v - b.maxPt[i]);
	}
	return sqDist;
}

bool CCollisionTests::TestSphereAABB(TSphere &s, TAABB &b)
{
	float sqDist = SqDistPointAABB(s.c, b);
	return sqDist <= s.r * s.r;
}

bool CCollisionTests::TestSphereSphere(TSphere &a, TSphere &b)
{
	D3DXVECTOR3 d = a.c - b.c;
	float dist2 = D3DXVec3Dot(&d, &d);
	float radiusSum = a.r + b.r;
	return dist2 <= (radiusSum*radiusSum);
}

bool CCollisionTests::TestSpherePlane(TSphere &s, D3DXPLANE &p)
{
	float dist = D3DXPlaneDotCoord(&p, &s.c);
	return abs(dist) <= s.r;
}

bool CCollisionTests::TestRayMesh(D3DXVECTOR3 &start, D3DXVECTOR3 &dir, CMesh *pMesh,
								  D3DXMATRIX mxWorld, D3DXVECTOR3 &vF, float &fDist, D3DXVECTOR3 *n)
{
	VertexPNT *pVerts;
	pMesh->GetVertexBuffer()->Lock(0,0, (void**)&pVerts, D3DLOCK_READONLY);

	int *pIndices;
	pMesh->GetIndexBuffer()->Lock(0,0, (void**)&pIndices, D3DLOCK_READONLY);

	float fU, fV;
	for( DWORD i = 0; i < pMesh->GetPrimitiveCount(); i++ )
	{
		D3DXVECTOR3 &v0 = pVerts[pIndices[3 * i + 0]].pos;
		D3DXVECTOR3 &v1 = pVerts[pIndices[3 * i + 1]].pos;
		D3DXVECTOR3 &v2 = pVerts[pIndices[3 * i + 2]].pos;

		// Check if the Pick ray passes through this point
		if( D3DXIntersectTri(&v0, &v1, &v2, &start, &dir, &fU, &fV, &fDist))
		{
			vF = v0 + fU*(v1 - v0) + fV*(v2 - v0);

			if(n)
			{
				D3DXVECTOR3 nv1 = v1 - v0;
				D3DXVECTOR3 nv2 = v2 - v0;
				D3DXVec3Cross(n, &nv1, &nv2);
				D3DXVec3Normalize(n, n);
			}

			pMesh->GetIndexBuffer()->Unlock();
			pMesh->GetVertexBuffer()->Unlock();
			return true;
		}
	}

	pMesh->GetIndexBuffer()->Unlock();
	pMesh->GetVertexBuffer()->Unlock();
	return false;
}

bool CCollisionTests::IntersectMovingAABBAABB(TAABB &a, TAABB &b, D3DXVECTOR3 &va, D3DXVECTOR3 &vb,
											  float &tfirst, float &tlast)
{
	if (TestAABBAABB(a, b)) {
		tfirst = tlast = 0.0f;
		return true;
	}
	// Use relative velocity; effectively treating ’a’ as stationary
	D3DXVECTOR3 v = vb - va;

	// Initialize times of first and last contact
	tfirst = 0.0f;
	tlast = 1.0f;

	// For each axis, determine times of first and last contact, if any
	for (int i = 0; i < 3; i++) {
		if (v[i] < 0.0f) {
			if (b.maxPt[i] < a.minPt[i]) return false; // Nonintersecting and moving apart
			if (a.maxPt[i] < b.minPt[i]) tfirst = max((a.maxPt[i] - b.minPt[i]) / v[i], tfirst);
			if (b.maxPt[i] > a.minPt[i]) tlast = min((a.minPt[i] - b.maxPt[i]) / v[i], tlast);
		}
		if (v[i] > 0.0f) {
			if (b.minPt[i] > a.maxPt[i]) return false; // Nonintersecting and moving apart
			if (b.maxPt[i] < a.minPt[i]) tfirst = max((a.minPt[i] - b.maxPt[i]) / v[i], tfirst);
			if (a.maxPt[i] > b.minPt[i]) tlast = min((a.maxPt[i] - b.minPt[i]) / v[i], tlast);
		}
		// No overlap possible if time of first contact occurs after time of last contact
		if (tfirst > tlast) return false;
	}
	return true;
}

bool CCollisionTests::TestAABBMesh(TAABB &a, CMesh *pMesh, D3DXMATRIX mxWorld, float &fDist)
{
	return true;
}

bool CCollisionTests::TestFrustrumSphere(TSphere &s, D3DXPLANE *f)
{
	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&f[i], &s.c) < -s.r)
			return false;
	}

	return true;
}

bool CCollisionTests::TestFrustrumSphere2(TSphere &s, D3DXPLANE *f)
{
	for(int i=0; i<2; i++)
	{
		if(D3DXPlaneDotCoord(&f[i], &s.c) < -s.r)
			return false;
	}

	return true;
}

int CCollisionTests::IntersectFrustrumSphere(TSphere &s, D3DXPLANE *f)
{
	for(int i=0; i<6; i++)
	{
		float dist = D3DXPlaneDotCoord(&f[i], &s.c);
		if( dist < -s.r)
			return CT_OUT;

		if( abs(dist) <= s.r)
			return CT_INTERSECT;
	}

	return CT_IN;
}

int CCollisionTests::IntersectFrustrumSphere2(TSphere &s, D3DXPLANE *f)
{
	for(int i=0; i<2; i++)
	{
		float dist = D3DXPlaneDotCoord(&f[i], &s.c);
		if( dist < -s.r)
			return CT_OUT;

		if( abs(dist) <= s.r)
			return CT_INTERSECT;
	}

	return CT_IN;
}

// broken, dont use
int CCollisionTests::IntersectFrustrumAABB(TAABB &a, D3DXPLANE *f)
{
	int ret = CT_IN;
	D3DXVECTOR3 vmin, vmax;

	for(int i = 0; i < 6; ++i)
	{
		D3DXVECTOR3 normal(f[i].a, f[i].b, f[i].c);

		// X axis
		if(normal.x > 0) {	vmin.x = a.minPt.x; 	vmax.x = a.maxPt.x;	}
		else {	vmin.x = a.maxPt.x;	vmax.x = a.minPt.x;	}

		// Y axis
		if(normal.y > 0) {	vmin.y = a.minPt.y;		vmax.y = a.maxPt.y;	}
		else {	vmin.y = a.maxPt.y;	vmax.y = a.minPt.y;	}

		// Z axis
		if(normal.z > 0) {	vmin.z = a.minPt.z;		vmax.z = a.maxPt.z;	}
		else {	vmin.z = a.maxPt.z;	vmax.z = a.minPt.z;	}

		if(D3DXVec3Dot(&normal, &vmin) + f[i].d > 0)
			return CT_OUT;
		if(D3DXVec3Dot(&normal, &vmax) + f[i].d >= 0)
			ret = CT_INTERSECT;
	}

	return ret;
}

bool CCollisionTests::TestFrustrumAABB(TAABB &a, D3DXPLANE *f)
{
	D3DXVECTOR3 c = (a.maxPt + a.minPt) * 0.5f;
	D3DXVECTOR3 e = a.maxPt - c;

	for(int i = 0; i < 6; i++)
	{
		float r = e.x*abs(f[i].a) + e.y*abs(f[i].b) + e.z*abs(f[i].c);
		float s = D3DXPlaneDotCoord(&f[i], &c);

		if(abs(s) > r)
		{
			if(s < 0)
				return false;
		}
	}

	return true;
}

bool CCollisionTests::TestFrustrumAABB2(TAABB &a, D3DXPLANE *f)
{
	D3DXVECTOR3 c = (a.maxPt + a.minPt) * 0.5f;
	D3DXVECTOR3 e = a.maxPt - c;

	for(int i = 0; i < 2; i++)
	{
		float r = e.x*abs(f[i].a) + e.y*abs(f[i].b) + e.z*abs(f[i].c);
		float s = D3DXPlaneDotCoord(&f[i], &c);

		if(abs(s) > r)
		{
			if(s < 0)
				return false;
		}
	}

	return true;
}

void CCollisionTests::MergeAABBAABB(TAABB &a, TAABB &a0, TAABB &a1)
{
	for (int i = 0; i < 3; i++) {
		a.minPt[i] = min(a0.minPt[i], a1.minPt[i]);
		a.maxPt[i] = max(a0.maxPt[i], a1.maxPt[i]);
	}
}

void CCollisionTests::MergeSphereSphere(TSphere &s, TSphere &s0, TSphere &s1)
{
	// Compute the squared distance between the sphere centers
	D3DXVECTOR3 d = s1.c - s0.c;
	float dist2 = D3DXVec3Dot(&d, &d);
	if (((s1.r - s0.r)*(s1.r - s0.r)) >= dist2) {
		// The sphere with the larger radius encloses the other;
		// just set s to be the larger of the two spheres
			if (s1.r >= s0.r)
				s = s1;
			else
				s = s0;
	} else {
		// Spheres partially overlapping or disjoint
		float dist = sqrt(dist2);
		s.r = (dist + s0.r + s1.r) * 0.5f;
		s.c = s0.c;
		if (dist > EPSILON)
			s.c += ((s.r - s0.r) / dist) * d;
	}
}

bool CCollisionTests::TestSegmentMesh( D3DXVECTOR3 &start, D3DXVECTOR3 &end, CMesh *pMesh, D3DXMATRIX mxWorld, D3DXVECTOR3 &vF, float &t )
{
	void *pVerts;
	pMesh->GetVertexBuffer()->Lock(0,0, &pVerts, D3DLOCK_READONLY);

	int *pIndices;
	pMesh->GetIndexBuffer()->Lock(0,0, (void**)&pIndices, D3DLOCK_READONLY);

	VertexPNT *newVerts = new VertexPNT[pMesh->GetVertexCount()];
	D3DXVec3TransformCoordArray(&newVerts[0].pos, pMesh->GetVertexStride(), (D3DXVECTOR3*)pVerts, pMesh->GetVertexStride(),
		&mxWorld, pMesh->GetVertexCount());

	float fU, fV, fW;
	for( DWORD i = 0; i < pMesh->GetPrimitiveCount(); i++ )
	{
		D3DXVECTOR3 &v0 = newVerts[pIndices[3 * i + 0]].pos;
		D3DXVECTOR3 &v1 = newVerts[pIndices[3 * i + 1]].pos;
		D3DXVECTOR3 &v2 = newVerts[pIndices[3 * i + 2]].pos;

		// Check if the Pick ray passes through this point
		if( CCollisionTests::IntersectSegmentTriangle(start, end, v2, v1, v0, fU, fV, fW, t))
		{
			vF = fU*v0 + fV*v1 + fW*v2;

			delete newVerts;
			pMesh->GetIndexBuffer()->Unlock();
			pMesh->GetVertexBuffer()->Unlock();
			return true;
		}
	}

	delete newVerts;
	pMesh->GetIndexBuffer()->Unlock();
	pMesh->GetVertexBuffer()->Unlock();
	return false;
}

bool CCollisionTests::IntersectSegmentTriangle( D3DXVECTOR3 p, D3DXVECTOR3 q, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, float &u, float &v, float &w, float &t )
{
	D3DXVECTOR3 ab = b - a;
	D3DXVECTOR3 ac = c - a;
	D3DXVECTOR3 qp = p - q;

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &ab, &ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = D3DXVec3Dot(&qp, &n);
	if (d <= 0.0f) return false;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	D3DXVECTOR3 ap = p - a;
	t = D3DXVec3Dot(&ap, &n);
	if (t < 0.0f) return false;
	if (t > d) return false; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	D3DXVECTOR3 e;
	D3DXVec3Cross(&e, &qp, &ap);

	v = D3DXVec3Dot(&ac, &e);
	if (v < 0.0f || v > d) return false;
	w = -D3DXVec3Dot(&ab, &e);
	if (w < 0.0f || v + w > d) return false;

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	float ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;
	u = 1.0f - v - w;
	return true;
}

bool CCollisionTests::TestAABBMesh(TAABB &a, CMesh *pMesh, D3DXMATRIX mxWorld)
{
	void *pVerts;
	pMesh->GetVertexBuffer()->Lock(0,0, &pVerts, D3DLOCK_READONLY);

	int *pIndices;
	pMesh->GetIndexBuffer()->Lock(0,0, (void**)&pIndices, D3DLOCK_READONLY);

	VertexPNT *newVerts = new VertexPNT[pMesh->GetVertexCount()];
	D3DXVec3TransformCoordArray(&newVerts[0].pos, pMesh->GetVertexStride(), (D3DXVECTOR3*)pVerts, pMesh->GetVertexStride(),
		&mxWorld, pMesh->GetVertexCount());

	for( DWORD i = 0; i < pMesh->GetPrimitiveCount(); i++ )
	{
		D3DXVECTOR3 &v0 = newVerts[pIndices[3 * i + 0]].pos;
		D3DXVECTOR3 &v1 = newVerts[pIndices[3 * i + 1]].pos;
		D3DXVECTOR3 &v2 = newVerts[pIndices[3 * i + 2]].pos;

		if( CCollisionTests::TestAABBTriangle(v0, v1, v2, a))
		{
			delete newVerts;
			pMesh->GetIndexBuffer()->Unlock();
			pMesh->GetVertexBuffer()->Unlock();
			return true;
		}
	}

	delete newVerts;
	pMesh->GetIndexBuffer()->Unlock();
	pMesh->GetVertexBuffer()->Unlock();
	return false;
}

bool CCollisionTests::TestAABBTriangle(D3DXVECTOR3 &v0, D3DXVECTOR3 &v1, D3DXVECTOR3 &v2, TAABB b)
{
	float p0, p1, p2, r;
	// Compute box center and extents (if not already given in that format)
	D3DXVECTOR3 c = b.center;
	float e0 = (b.maxPt.x - b.minPt.x) * 0.5f;
	float e1 = (b.maxPt.y - b.minPt.y) * 0.5f;
	float e2 = (b.maxPt.z - b.minPt.z) * 0.5f;
	// Translate triangle as conceptually moving AABB to origin
	v0 = v0 - c;
	v1 = v1 - c;
	v2 = v2 - c;
	// Compute edge vectors for triangle
	D3DXVECTOR3 f0 = v1 - v0, f1 = v2 - v1, f2 = v0 - v2;

	// Test axes a00..a22 (category 3)

	// Test axis a00
	p0 = v0.z*v1.y - v0.y*v1.z;
	p2 = v2.z*(v1.y - v0.y) - v2.z*(v1.z - v0.z);
	r = e1 * abs(f0.z) + e2 * abs(f0.y);
	if (max(-max(p0, p2), min(p0, p2)) > r) return false; // Axis is a separating axis
	// Repeat similar tests for remaining axes a01..a22

	D3DXVECTOR3 a01 = D3DXVECTOR3(0.0f, -f1.z, f1.y);
	D3DXVECTOR3 a02 = D3DXVECTOR3(0.0f, -f2.z, f2.y);
	D3DXVECTOR3 a10 = D3DXVECTOR3(f0.z, 0.0f, -f0.x);
	D3DXVECTOR3 a11 = D3DXVECTOR3(f1.z, 0.0f, -f1.x);
	D3DXVECTOR3 a12 = D3DXVECTOR3(f2.z, 0.0f, -f2.x);
	D3DXVECTOR3 a20 = D3DXVECTOR3(-f0.y, f0.x, 0.0f);
	D3DXVECTOR3 a21 = D3DXVECTOR3(-f1.y, f1.x, 0.0f);
	D3DXVECTOR3 a22 = D3DXVECTOR3(-f2.y, f2.x, 0.0f);

	//a01
	p0 = D3DXVec3Dot(&v0, &a01);
	p1 = D3DXVec3Dot(&v1, &a01);
	p2 = D3DXVec3Dot(&v2, &a01);
	r = e1 * abs(f1.z) + e2 * abs(f1.y);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a02
	p0 = D3DXVec3Dot(&v0, &a02);
	p1 = D3DXVec3Dot(&v1, &a02);
	p2 = D3DXVec3Dot(&v2, &a02);
	r = e1 * abs(f2.z) + e2 * abs(f2.y);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a10
	p0 = D3DXVec3Dot(&v0, &a10);
	p1 = D3DXVec3Dot(&v1, &a10);
	p2 = D3DXVec3Dot(&v2, &a10);
	r = e0 * abs(f0.z) + e2 * abs(f0.x);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a11
	p0 = D3DXVec3Dot(&v0, &a11);
	p1 = D3DXVec3Dot(&v1, &a11);
	p2 = D3DXVec3Dot(&v2, &a11);
	r = e0 * abs(f1.z) + e2 * abs(f1.x);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a12
	p0 = D3DXVec3Dot(&v0, &a12);
	p1 = D3DXVec3Dot(&v1, &a12);
	p2 = D3DXVec3Dot(&v2, &a12);
	r = e0 * abs(f2.z) + e2 * abs(f2.x);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a20
	p0 = D3DXVec3Dot(&v0, &a20);
	p1 = D3DXVec3Dot(&v1, &a20);
	p2 = D3DXVec3Dot(&v2, &a20);
	r = e0 * abs(f0.y) + e1 * abs(f0.x);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a21
	p0 = D3DXVec3Dot(&v0, &a21);
	p1 = D3DXVec3Dot(&v1, &a21);
	p2 = D3DXVec3Dot(&v2, &a21);
	r = e0 * abs(f1.y) + e1 * abs(f1.x);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	//a22
	p0 = D3DXVec3Dot(&v0, &a22);
	p1 = D3DXVec3Dot(&v1, &a22);
	p2 = D3DXVec3Dot(&v2, &a22);
	r = e0 * abs(f2.y) + e1 * abs(f2.x);
	if (max(-MAX3(p0, p1, p2), MIN3(p0, p1, p2)) > r) return false; // Axis is a separating axis

	// Test the three axes corresponding to the face normals of AABB b (category 1).
	// Exit if...
	// ... [-e0, e0] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap
	if (MAX3(v0.x, v1.x, v2.x) < -e0 || MIN3(v0.x, v1.x, v2.x) > e0) return false;
	// ... [-e1, e1] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
	if (MAX3(v0.y, v1.y, v2.y) < -e1 || MIN3(v0.y, v1.y, v2.y) > e1) return false;
	// ... [-e2, e2] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
	if (MAX3(v0.z, v1.z, v2.z) < -e2 || MIN3(v0.z, v1.z, v2.z) > e2) return false;
	// Test separating axis corresponding to triangle face normal (category 2)
	D3DXPLANE p;
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &f0, &f1);
	p.a = n.x; p.b = n.y; p.c = n.z;
	p.d = D3DXVec3Dot(&n, &v0);

	TAABB b2;
	b2.minPt = b.minPt - c;
	b2.maxPt = b.maxPt - c;
	return TestAABBPlane(b2, p);
	//return TestAABBPlane(b, p);
}

bool CCollisionTests::IntersectSegmentPlane( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXPLANE p, float &t, D3DXVECTOR3 &q )
{
	D3DXVECTOR3 n(p.a, p.b, p.c);
	// Compute the t value for the directed line ab intersecting the plane
	D3DXVECTOR3 ab = b - a;
	t = (p.d - D3DXVec3Dot(&n, &a)) / D3DXVec3Dot(&n, &ab);
	// If t in [0..1] compute and return intersection point
	if (t >= 0.0f && t <= 1.0f) {
		q = a + t * ab;
		return true;
	}
	// Else no intersection
	return false;
}

bool CCollisionTests::TestOBBOBB( TOBB &a, TOBB &b )
{
	float ra, rb;
	D3DXMATRIX R, AbsR;

	// Compute rotation matrix expressing b in a?s coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R.m[i][j] = D3DXVec3Dot(&a.u[i], &b.u[j]);

	// Compute translation vector t
	D3DXVECTOR3 t = b.c - a.c;

	// Bring translation into a?s coordinate frame
	t = D3DXVECTOR3(D3DXVec3Dot(&t, &a.u[0]), D3DXVec3Dot(&t, &a.u[1]), D3DXVec3Dot(&t, &a.u[2]));

	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR.m[i][j] = abs(R.m[i][j]) + EPSILON;

	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++) {
		ra = a.e[i];
		rb = b.e[0] * AbsR.m[i][0] + b.e[1] * AbsR.m[i][1] + b.e[2] * AbsR.m[i][2];
		if (abs(t[i]) > ra + rb) return false;
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++) {
		ra = a.e[0] * AbsR.m[0][i] + a.e[1] * AbsR.m[1][i] + a.e[2] * AbsR.m[2][i];
		rb = b.e[i];
		if (abs(t[0] * R.m[0][i] + t[1] * R.m[1][i] + t[2] * R.m[2][i]) > ra + rb) return false;
	}

	// Test axis L = A0 x B0
	ra = a.e[1] * AbsR.m[2][0] + a.e[2] * AbsR.m[1][0];
	rb = b.e[1] * AbsR.m[0][2] + b.e[2] * AbsR.m[0][1];
	if (abs(t[2] * R.m[1][0] - t[1] * R.m[2][0]) > ra + rb) return false;

	// Test axis L = A0 x B1
	ra = a.e[1] * AbsR.m[2][1] + a.e[2] * AbsR.m[1][1];
	rb = b.e[0] * AbsR.m[0][2] + b.e[2] * AbsR.m[0][0];
	if (abs(t[2] * R.m[1][1] - t[1] * R.m[2][1]) > ra + rb) return false;

	// Test axis L = A0 x B2
	ra = a.e[1] * AbsR.m[2][2] + a.e[2] * AbsR.m[1][2];
	rb = b.e[0] * AbsR.m[0][1] + b.e[1] * AbsR.m[0][0];
	if (abs(t[2] * R.m[1][2] - t[1] * R.m[2][2]) > ra + rb) return false;

	// Test axis L = A1 x B0
	ra = a.e[0] * AbsR.m[2][0] + a.e[2] * AbsR.m[0][0];
	rb = b.e[1] * AbsR.m[1][2] + b.e[2] * AbsR.m[1][1];
	if (abs(t[0] * R.m[2][0] - t[2] * R.m[0][0]) > ra + rb) return false;

	// Test axis L = A1 x B1
	ra = a.e[0] * AbsR.m[2][1] + a.e[2] * AbsR.m[0][1];
	rb = b.e[0] * AbsR.m[1][2] + b.e[2] * AbsR.m[1][0];
	if (abs(t[0] * R.m[2][1] - t[2] * R.m[0][1]) > ra + rb) return false;

	// Test axis L = A1 x B2
	ra = a.e[0] * AbsR.m[2][2] + a.e[2] * AbsR.m[0][2];
	rb = b.e[0] * AbsR.m[1][1] + b.e[1] * AbsR.m[1][0];
	if (abs(t[0] * R.m[2][2] - t[2] * R.m[0][2]) > ra + rb) return false;

	// Test axis L = A2 x B0
	ra = a.e[0] * AbsR.m[1][0] + a.e[1] * AbsR.m[0][0];
	rb = b.e[1] * AbsR.m[2][2] + b.e[2] * AbsR.m[2][1];
	if (abs(t[1] * R.m[0][0] - t[0] * R.m[1][0]) > ra + rb) return false;

	// Test axis L = A2 x B1
	ra = a.e[0] * AbsR.m[1][1] + a.e[1] * AbsR.m[0][1];
	rb = b.e[0] * AbsR.m[2][2] + b.e[2] * AbsR.m[2][0];
	if (abs(t[1] * R.m[0][1] - t[0] * R.m[1][1]) > ra + rb) return false;

	// Test axis L = A2 x B2
	ra = a.e[0] * AbsR.m[1][2] + a.e[1] * AbsR.m[0][2];
	rb = b.e[0] * AbsR.m[2][1] + b.e[1] * AbsR.m[2][0];
	if (abs(t[1] * R.m[0][2] - t[0] * R.m[1][2]) > ra + rb) return false;

	// Since no separating axis is found, the OBBs must be intersecting
	return true;
}

void CCollisionTests::ClosestPtPointOBB( D3DXVECTOR3 p, TOBB b, D3DXVECTOR3 &q )
{
	D3DXVECTOR3 d = p - b.c;
	// Start result at center of box; make steps from there
	q = b.c;
	// For each OBB axis...
	for (int i = 0; i < 3; i++) {
		// ...project d onto that axis to get the distance
		// along the axis of d from the box center
		float dist = D3DXVec3Dot(&d, &b.u[i]);
		// If distance farther than the box extents, clamp to the box
		if (dist > b.e[i]) dist = b.e[i];
		if (dist < -b.e[i]) dist = -b.e[i];
		// Step that distance along the axis to get world coordinate
		q += dist * b.u[i];
	}
}

bool CCollisionTests::TestSphereOBB( TSphere s, TOBB b, D3DXVECTOR3 &p )
{
	// Find point p on OBB closest to sphere center
	CCollisionTests::ClosestPtPointOBB(s.c, b, p);
	// Sphere and OBB intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	D3DXVECTOR3 v = p - s.c;
	return D3DXVec3Dot(&v, &v) <= s.r * s.r;
}

float CCollisionTests::Clamp( float n, float min, float max )
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}
bool CCollisionTests::TestAABBOBB( TAABB &a, TOBB &b )
{
	return false;
}

float CCollisionTests::SqDistPointSegment( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c )
{
	D3DXVECTOR3 ab = b - a, ac = c - a, bc = c - b;
	float e = D3DXVec3Dot(&ac, &ab);
	// Handle cases where c projects outside ab
	if (e <= 0.0f) return D3DXVec3Dot(&ac, &ac);
	float f = D3DXVec3Dot(&ab, &ab);
	if (e >= f) return D3DXVec3Dot(&bc, &bc);
	// Handle cases where c projects onto ab
	return D3DXVec3Dot(&ac, &ac) - e * e / f;
}

float CCollisionTests::ClosestPtSegmentSegment( D3DXVECTOR3 p1, D3DXVECTOR3 q1, D3DXVECTOR3 p2, D3DXVECTOR3 q2, float &s, float &t, D3DXVECTOR3 &c1, D3DXVECTOR3 &c2 )
{
	D3DXVECTOR3 d1 = q1 - p1; // Direction vector of segment S1
	D3DXVECTOR3 d2 = q2 - p2; // Direction vector of segment S2
	D3DXVECTOR3 r = p1 - p2;
	float a = D3DXVec3Dot(&d1, &d1); // Squared length of segment S1, always nonnegative
	float e = D3DXVec3Dot(&d2, &d2); // Squared length of segment S2, always nonnegative
	float f = D3DXVec3Dot(&d2, &r);
	// Check if either or both segments degenerate into points
	if (a <= EPSILON && e <= EPSILON) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;

		D3DXVECTOR3 c2c1 = c1 - c2;
		return D3DXVec3Dot(&c2c1, &c2c1);
	}
	if (a <= EPSILON) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e; // s = 0 => t = (b*s + f) / e = f / e
		t = Clamp(t, 0.0f, 1.0f);
	} else {
		float c = D3DXVec3Dot(&d1, &r);
		if (e <= EPSILON) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
		} else {
			// The general nondegenerate case starts here
			float b = D3DXVec3Dot(&d1, &d2);
			float denom = a*e-b*b; // Always nonnegative
			// If segments not parallel, compute closest point on L1 to L2 and
			// clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f) {
				s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
			} else s = 0.0f;
			// Compute point on L2 closest to S1(s) using
			// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
			t = (b*s + f) / e;
			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (t < 0.0f) {
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			} else if (t > 1.0f) {
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;

	D3DXVECTOR3 c2c1 = c1 - c2;
	return D3DXVec3Dot(&c2c1, &c2c1);
}

bool CCollisionTests::TestSphereCapsule( TSphere &s, TCapsule &capsule )
{
	// Compute (squared) distance between sphere center and capsule line segment
	float dist2 = SqDistPointSegment(capsule.a, capsule.b, s.c);
	// If (squared) distance smaller than (squared) sum of radii, they collide
	float radius = s.r + capsule.r;
	return dist2 <= radius * radius;
}

bool CCollisionTests::TestCapsuleCapsule( TCapsule &capsule1, TCapsule &capsule2 )
{
	// Compute (squared) distance between the inner structures of the capsules
	float s, t;
	D3DXVECTOR3 c1, c2;
	float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b,
		capsule2.a, capsule2.b, s, t, c1, c2);
	// If (squared) distance smaller than (squared) sum of radii, they collide
	float radius = capsule1.r + capsule2.r;
	return dist2 <= radius * radius;
}

bool CCollisionTests::TestSegmentAABB( D3DXVECTOR3 &p0, D3DXVECTOR3 &p1, TAABB &b )
{
	D3DXVECTOR3 c = b.center; // Box center-point
	D3DXVECTOR3 e = b.maxPt - c; // Box halflength extents
	D3DXVECTOR3 m = (p0 + p1) * 0.5f; // Segment midpoint
	D3DXVECTOR3 d = p1 - m; // Segment halflength vector
	m = m - c; // Translate box and segment to origin
	// Try world coordinate axes as separating axes
	float adx = abs(d.x);
	if (abs(m.x) > e.x + adx) return 0;
	float ady = abs(d.y);
	if (abs(m.y) > e.y + ady) return 0;
	float adz = abs(d.z);
	if (abs(m.z) > e.z + adz) return 0;
	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis (see text for detail)
	adx += EPSILON; ady += EPSILON; adz += EPSILON;

	// Try cross products of segment direction vector with coordinate axes
	if (abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return false;
	if (abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return false;
	if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;
	// No separating axis found; segment must be overlapping AABB
	return true;
}

bool CCollisionTests::TestCapsuleAABB( TCapsule &c, TAABB &b )
{
	TAABB eb = b;
	D3DXVECTOR3 e(c.r, c.r, c.r);

	eb.minPt -= e;
	eb.maxPt += e;

	return CCollisionTests::TestSegmentAABB(c.a, c.b, eb);
}