#include "CMesh.h"

CMesh::CMesh()
{
	D3DPRIMITIVETYPE m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_PrimitiveCount = 0;

	m_VertexBuffer = 0;
	m_VertexStride = 0;
	m_VertexCount = 0;

	m_WeightBuffer = 0;
	m_WeightStride = 0;

	m_IndexBuffer = 0;
	m_IndexCount = 0;
	m_IndexStride = 0;

	m_nTextureID = -1;
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_WeightBuffer);
	SAFE_RELEASE(m_IndexBuffer);
}

void CMesh::BuildMesh(D3DPRIMITIVETYPE primType, unsigned int nPrimCount,
					  void *pVertData, unsigned int nVertCount, unsigned int nVertStride,
					  void *pIndexData, unsigned int nIdxCount, unsigned int nIdxStride,
					  void *pWeightData, unsigned int nWeightStride)
{
	m_PrimitiveType = primType;
	m_PrimitiveCount = nPrimCount;

	FillVertBuffer(pVertData, nVertCount, nVertStride);
	FillIndexBuffer(pIndexData, nIdxCount, nIdxStride);

	if(pWeightData)
		FillWeightBuffer(pWeightData, nVertCount, nWeightStride);

	m_AABB.CreateFromVerts((D3DXVECTOR3 *)pVertData, nVertCount, nVertStride);
	m_Sphere.CreateFromVerts((D3DXVECTOR3 *)pVertData, nVertCount, nVertStride);
}

void CMesh::FillVertBuffer(void *pVertData, unsigned int nVertCount, unsigned int nVertStride, DWORD dwFlags)
{
	SAFE_RELEASE(m_VertexBuffer);

	m_VertexCount = nVertCount;
	m_VertexStride = nVertStride;

	CDirect3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(nVertCount*nVertStride,
		dwFlags,
		0,
		D3DPOOL_MANAGED,
		&m_VertexBuffer,
		0);

	void* pBuffer;
	m_VertexBuffer->Lock(0,0, &pBuffer, 0);
	memcpy(pBuffer, pVertData, nVertCount*nVertStride);
	m_VertexBuffer->Unlock();
}

void CMesh::FillIndexBuffer(void *pIndexData, unsigned int nIdxCount, unsigned int nIdxStride, DWORD dwFlags)
{
	SAFE_RELEASE(m_IndexBuffer);

	m_IndexCount = nIdxCount;
	m_IndexStride = nIdxStride;

	CDirect3D::GetInstance()->GetDirect3DDevice()->CreateIndexBuffer(nIdxCount*nIdxStride,
		dwFlags,
		nIdxStride == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_IndexBuffer,
		0);

	void* pBuffer;
	m_IndexBuffer->Lock(0,0, &pBuffer, 0);
	memcpy(pBuffer, pIndexData, nIdxCount*nIdxStride);
	m_IndexBuffer->Unlock();
}

void CMesh::FillWeightBuffer(void *pWeightData, unsigned int nWeightCount, unsigned int nWeightStride, DWORD dwFlags)
{
	SAFE_RELEASE(m_WeightBuffer);

	m_WeightStride = nWeightStride;

	CDirect3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(nWeightCount*nWeightStride,
		dwFlags,
		0,
		D3DPOOL_MANAGED,
		&m_WeightBuffer,
		0);

	void* pBuffer;
	m_WeightBuffer->Lock(0,0, &pBuffer, 0);
	memcpy(pBuffer, pWeightData, nWeightCount*nWeightStride);
	m_WeightBuffer->Unlock();
}