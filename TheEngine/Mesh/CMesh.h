#pragma once

#include "../Rendering/CDirect3D.h"
#include "../Collision/CAABB.h"
#include "../Collision/CSphere.h"

class CMesh
{
	D3DPRIMITIVETYPE m_PrimitiveType;
	unsigned int m_PrimitiveCount;

	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;
	unsigned int m_VertexStride;
	unsigned int m_VertexCount;

	LPDIRECT3DVERTEXBUFFER9 m_WeightBuffer;
	unsigned int m_WeightStride;

	LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;
	unsigned int m_IndexCount;
	unsigned int m_IndexStride;

	int m_nTextureID;

	CAABB m_AABB;
	CSphere m_Sphere;

public:
	CMesh();
	~CMesh();

	D3DPRIMITIVETYPE &GetPrimitiveType() { return m_PrimitiveType; }
	unsigned int &GetPrimitiveCount() { return m_PrimitiveCount; }

	void SetPrimitiveType(D3DPRIMITIVETYPE primType) { m_PrimitiveType = primType; }
	void SetPrimitiveCount(unsigned int nPrimCount) { m_PrimitiveCount = nPrimCount; }

	LPDIRECT3DVERTEXBUFFER9 &GetVertexBuffer() { return m_VertexBuffer; }
	unsigned int &GetVertexStride() { return m_VertexStride; }
	unsigned int &GetVertexCount() { return m_VertexCount; }

	void SetVertexStride(unsigned int nVertStride) { m_VertexStride = nVertStride; }
	void SetVertexCount(unsigned int nVertCount) { m_VertexCount = nVertCount; }

	LPDIRECT3DVERTEXBUFFER9 &GetWeightBuffer() { return m_WeightBuffer; }
	unsigned int &GetWeightStride() { return m_WeightStride; }

	LPDIRECT3DINDEXBUFFER9 &GetIndexBuffer() { return m_IndexBuffer; }
	unsigned int &GetIndexCount() { return m_IndexCount; }
	unsigned int &GetIndexStride() { return m_IndexStride; }

	int GetTextureID() { return m_nTextureID; }
	void SetTextureID(int nID)	{ m_nTextureID = nID; }

	void BuildMesh(D3DPRIMITIVETYPE primType, unsigned int nPrimCount, void *pVertData, unsigned int nVertCount, unsigned int nVertStride,
					void *pIndexData, unsigned int nIdxCount, unsigned int nIdxStride,
					void *pWeightData = NULL, unsigned int nWeightStride = 0);

	void FillVertBuffer(void *pVertData, unsigned int nVertCount, unsigned int nVertStride, DWORD dwFlags = 0);
	void FillIndexBuffer(void *pIndexData, unsigned int nIdxCount, unsigned int nIdxStride, DWORD dwFlags = 0);
	void FillWeightBuffer(void *pWeightData, unsigned int nWeightCount, unsigned int nWeightStride, DWORD dwFlags = 0);

	CAABB &GetAABB() { return m_AABB; }
	CSphere &GetSphere() { return m_Sphere; }
};