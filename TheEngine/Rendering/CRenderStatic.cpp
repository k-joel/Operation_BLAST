#include "CRenderStatic.h"
#include "CRenderContext.h"
#include "../Mesh/CMesh.h"
#include "CDirect3D.h"
#include "CTextureManager.h"
#include "CRenderContext.h"

CRenderStatic::CRenderStatic()
{
	m_pRenderContext = NULL;
	m_pMesh = NULL;
	m_pData = NULL;
}

CRenderStatic::~CRenderStatic()
{
}

void CRenderStatic::Init(CMesh *pMesh, CRenderContext* pRC,	void (*RenderFunc)(CRenderNode&))
{
	m_pMesh = pMesh;
	m_pRenderContext = pRC;
	this->RenderFunc = RenderFunc;
}

void CRenderStatic::PCRenderFunc(CRenderNode &node)
{
	CRenderStatic &rs = (CRenderStatic&)node;

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPC::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderStatic::PNTRenderFunc(CRenderNode &node)
{
	CRenderStatic &rs = (CRenderStatic&)node;

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNT::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}