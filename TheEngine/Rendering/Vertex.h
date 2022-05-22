//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "Vertex.h"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Creates the vertex declarations						 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CDirect3D.h"

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

//===============================================================
struct VertexPos
{
	VertexPos():pos(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z):pos(x,y,z){}
	VertexPos(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPC
{
	VertexPC():pos(0.0f, 0.0f, 0.0f),col(0x00000000){}
	VertexPC(float x, float y, float z, D3DCOLOR c):pos(x,y,z), col(c){}
	VertexPC(const D3DXVECTOR3& v, D3DCOLOR c):pos(v),col(c){}

	D3DXVECTOR3 pos;
	D3DCOLOR    col;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPN
{
	VertexPN()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f){}
	VertexPN(float x, float y, float z,
		float nx, float ny, float nz):pos(x,y,z), normal(nx,ny,nz){}
	VertexPN(const D3DXVECTOR3& v, const D3DXVECTOR3& n)
		:pos(v),normal(n){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNT
{
	VertexPNT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f){}
	VertexPNT(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v):pos(x,y,z), normal(nx,ny,nz), tex0(u,v){}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
		:pos(v),normal(n), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNTT
{
	VertexPNTT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f),
		tex1(0.0f, 0.0f){}
	VertexPNTT(float x, float y, float z,
		float nx, float ny, float nz,
		float u0, float v0,
		float u1, float v1):pos(x,y,z), normal(nx,ny,nz), tex0(u0,v0), tex1(u1,v1) {}
	VertexPNTT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv0, const D3DXVECTOR2& uv1)
		:pos(v),normal(n), tex0(uv0), tex1(uv1) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;
	D3DXVECTOR2 tex1;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNCT
{
	VertexPNCT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		color(1.0f, 1.0f, 1.0f, 1.0f),
		tex0(0.0f, 0.0f){}
	VertexPNCT(float x, float y, float z,
		float nx, float ny, float nz,
		float r, float g, float b, float a,
		float u, float v):pos(x,y,z), normal(nx,ny,nz), color(r, g, b, a), tex0(u,v){}
	VertexPNCT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXCOLOR& c, const D3DXVECTOR2& uv)
		:pos(v),normal(n), color(c), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXCOLOR color;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNTBB
{
	VertexPNTBB()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f),
		blendweight(0.0f, 0.0f, 0.0f, 0.0f),
		blendindices(0.0f, 0.0f, 0.0f, 0.0f){}
	VertexPNTBB(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v,
		float bw1, float bw2, float bw3, float bw4,
		float bi1, float bi2, float bi3, float bi4):pos(x,y,z), normal(nx,ny,nz), tex0(u,v),
											blendweight(bw1, bw2, bw3, bw4), blendindices(bi1, bi2, bi3, bi4) {}
	VertexPNTBB(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv,
			const D3DXVECTOR4& bw, const D3DXVECTOR4& bi)
		:pos(v),normal(n), tex0(uv), blendweight(bw), blendindices(bi) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;
	D3DXVECTOR4 blendweight;
	D3DXVECTOR4	blendindices;

	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexParticle
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 StartVel;
	D3DXVECTOR3 EndVel;
	D3DXVECTOR3 StartAcc;
	D3DXVECTOR3 EndAcc;
	D3DXVECTOR3 OrbitLoc;
	D3DXVECTOR3 OrbitRot;
	D3DXVECTOR3 OrbitRotRate;
	D3DXVECTOR4 Life;
	D3DXVECTOR4 Rot;
	D3DXVECTOR4 Size;
	D3DCOLOR    StartColor;
	D3DCOLOR	EndColor;

	VertexParticle() : Pos(0.0f, 0.0f, 0.0f),
						StartVel(0.0f, 0.0f, 0.0f),
						EndVel(0.0f, 0.0f, 0.0f),
						StartAcc(0.0f, 0.0f, 0.0f),
						EndAcc(0.0f, 0.0f, 0.0f),
						OrbitLoc(0.0f, 0.0f, 0.0f),
						OrbitRot(0.0f, 0.0f, 0.0f),
						OrbitRotRate(0.0f, 0.0f, 0.0f),
						Life(0.0f, 1.0f, 0.0f, 0.0f),
						Rot(0.0f, 0.0f, 0.0f, 0.0f),
						Size(64.0f, 64.0f, 0.0f, 0.0f),
						StartColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),
						EndColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) {}

	static IDirect3DVertexDeclaration9* Decl;
};

//--------------------------------------------------------------------------------------
// This is the vertex format used with the quad during post-process.
struct VertexPP
{
	VertexPP()
		: post(0.0f, 0.0f, 0.0f, 0.0f),
		tuv(0.0f, 0.0f),
		tuv2(0.0f, 0.0f) {}

	D3DXVECTOR4 post;
	D3DXVECTOR2 tuv; // Texcoord for post-process source
	D3DXVECTOR2 tuv2; // Texcoord for the original scene

	static IDirect3DVertexDeclaration9* Decl;
};