#pragma once
#include "../Core/Util.h"
#include "../Rendering/CDirect3D.h"

struct TPlane
{
	D3DXVECTOR3 n;
	float d;
};

struct TAABB
{
	// Initialize to an infinitely small bounding box.
	TAABB()	: minPt(INFINITY, INFINITY, INFINITY),
			maxPt(-INFINITY, -INFINITY, -INFINITY),
			center(0.0f, 0.0f, 0.0f) {}

	D3DXVECTOR3 minPt;
	D3DXVECTOR3 maxPt;
	D3DXVECTOR3 center;
};

struct TSphere
{
	D3DXVECTOR3 c;
	float r;

	TSphere() : c(0.0f, 0.0f, 0.0f), r(0.0f) {}
};

struct TOBB
{
	D3DXVECTOR3 c;
	D3DXVECTOR3 u[3];
	D3DXVECTOR3 e;

	TOBB() : c(0.0f, 0.0f, 0.0f),
			e(0.0f, 0.0f, 0.0f) {}
};

struct TCapsule
{
	D3DXVECTOR3 a;
	D3DXVECTOR3 b;
	float r;

	TCapsule() : a(0.0f, 0.0f, 0.0f), b(0.0f, 0.0f, 0.0f), r(0.0f) {}
};