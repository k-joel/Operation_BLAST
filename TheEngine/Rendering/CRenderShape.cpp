#include "CRenderShape.h"
#include "CRenderContext.h"
#include "../Mesh/CMesh.h"
#include "CDirect3D.h"
#include "CTextureManager.h"
#include "../Mesh/CModel.h"
#include "../Camera/Camera.h"
#include "../States/CGameState.h"
#include "../Core/CGame.h"

CRenderShape::CRenderShape()
{
	m_tMtrl.m_Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.m_Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMesh = NULL;
	m_pModel = NULL;
}

CRenderShape::~CRenderShape()
{
}

void CRenderShape::Init(CMesh *pMesh, CModel *pModel, CRenderContext* pRC, D3DXMATRIX &mxWorld,
						void (*RenderFunc)(CRenderNode&))
{
	m_pMesh = pMesh;
	if(pModel)
	{
		m_pModel = pModel;
		pModel->duplicateControler(m_pAC);
	}
	m_nDiffuseTexID = m_pMesh->GetTextureID();
	m_pRenderContext = pRC;
	m_Frame.GetLocalMatrix() = mxWorld;
	this->RenderFunc = RenderFunc;
}

void CRenderShape::Init(CModel *pModel, D3DXMATRIX &mxWorld, CRenderContext* pRC, void (*RenderFunc)(CRenderNode&))
{
	m_pModel = pModel;
	m_pMesh = &pModel->GetMesh();
	pModel->duplicateControler(m_pAC);
	m_nDiffuseTexID = m_pMesh->GetTextureID();
	m_pRenderContext = pRC;
	m_Frame.GetLocalMatrix() = mxWorld;
	this->RenderFunc = RenderFunc;
}

void CRenderShape::Init(CMesh *pMesh, D3DXMATRIX &mxWorld, CRenderContext* pRC, void (*RenderFunc)(CRenderNode&))
{
	m_pMesh = pMesh;
	m_nDiffuseTexID = m_pMesh->GetTextureID();
	m_pRenderContext = pRC;
	m_Frame.GetLocalMatrix() = mxWorld;
	this->RenderFunc = RenderFunc;
}

void CRenderShape::PCRenderFunc(CRenderNode &node)
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPC::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::PNTRenderFunc(CRenderNode &node)
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));
	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNT::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::ShadowPNTRenderFunc(CRenderNode &node)
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));

	D3DXMATRIX worldView = rs.GetFrame().GetWorldMatrix() * CCamera::GetInstance()->GetViewMatrix();
	rs.m_pRenderContext->SetWorldView(worldView);
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNT::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::PNTTRenderFunc(CRenderNode &node)
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));
	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNTT::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::PNTBBRenderFunc(CRenderNode &node)
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));
	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());
	rs.m_pRenderContext->SetMatrixPalette(rs.GetAnimCtrl().getMatrixPalet(), rs.GetAnimCtrl().getNumberOfMatrixsInPalet());
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNTBB::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(1, rs.GetMesh()->GetWeightBuffer(), 0, rs.GetMesh()->GetWeightStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::ParticleRenderFunc(CRenderNode &node)
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetMesh()->GetTextureID()));
	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());

	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexParticle::Decl);
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::DepthPNTRenderFunc( CRenderNode &node, CRenderNode &contextNode )
{
	//CRenderContext &context = (CRenderContext &)contextNode;

	//CRenderShape &rs = (CRenderShape&)node;

	//D3DXMATRIX worldView; //CCamera::GetInstance()->GetViewMatrix();
	//D3DXMatrixMultiply(&worldView, &rs.GetFrame().GetWorldMatrix(), &CRenderContextManager::GetInstance()->GetLightView());
	//context.SetWorldView(worldView);
	//context.SetProj(CRenderContextManager::GetInstance()->GetShadowProj());
	//context.CommitChanges();

	//CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNT::Decl);

	//CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	//CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	//CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
	//	rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::UVScrollPNTRenderFunc( CRenderNode &node )
{
	CRenderShape &rs = (CRenderShape&)node;

	if(rs.GetData())
	{
		TUVScrollData *data = (TUVScrollData *)rs.GetData();
		rs.m_pRenderContext->GetEffect()->SetFloat("g_fRate", data->fRate);
		rs.m_pRenderContext->GetEffect()->SetValue("g_vDirection", &data->vDirection, sizeof(D3DXVECTOR2));
	}
	else
	{
		rs.m_pRenderContext->GetEffect()->SetFloat("g_fRate", 1.0f);
		rs.m_pRenderContext->GetEffect()->SetValue("g_vDirection", &(D3DXVECTOR2(1.0f, 0.0f)), sizeof(D3DXVECTOR2));
	}

	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));
	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNT::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());
}

void CRenderShape::UVAtlasPNTRenderFunc( CRenderNode &node )
{
	CRenderShape &rs = (CRenderShape&)node;

	rs.m_pRenderContext->GetEffect()->SetFloat("g_fTime", CGameState::GetInstance()->GetGameTime());
	rs.m_pRenderContext->GetEffect()->SetBool("g_bUseAtlas", true);

	if(rs.GetData())
	{
		TUVAtlasData *data = (TUVAtlasData *)rs.GetData();
		rs.m_pRenderContext->GetEffect()->SetFloat("g_fRate", data->fRate);
		rs.m_pRenderContext->GetEffect()->SetValue("g_vDim", &data->vDimension, sizeof(D3DXVECTOR2));

		if(rs.m_fDelay > 0.0f)
		{
			rs.m_fCurTime += CGame::GetInstance()->GetElapsedTime();
			if(rs.m_fCurTime > rs.m_fDelay)
			{
				if((rs.m_fCurTime - rs.m_fDelay) > (data->vDimension.x*data->vDimension.y)/data->fRate)
				{
					rs.m_fCurTime = 0.0f;
					goto l_end;
				}
				goto l_begin;
			}
			goto l_end;
		}
		goto l_begin;
	}
	else
	{
		rs.m_pRenderContext->GetEffect()->SetFloat("g_fRate", 1.0f);
		rs.m_pRenderContext->GetEffect()->SetValue("g_vDim", &(D3DXVECTOR2(2.0f, 2.0f)), sizeof(D3DXVECTOR2));
		goto l_begin;
	}

l_end:
	rs.m_pRenderContext->GetEffect()->SetBool("g_bUseAtlas", false);
	return;

l_begin:
	rs.m_pRenderContext->SetTexture(CTextureManager::GetInstance()->GetTexture(rs.GetTexID()));
	rs.m_pRenderContext->SetWorld(rs.GetFrame().GetWorldMatrix());
	rs.m_pRenderContext->CommitChanges();

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetVertexDeclaration(VertexPNT::Decl);

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetStreamSource(0, rs.GetMesh()->GetVertexBuffer(), 0, rs.GetMesh()->GetVertexStride());
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetIndices(rs.GetMesh()->GetIndexBuffer());

	CDirect3D::GetInstance()->GetDirect3DDevice()->DrawIndexedPrimitive(rs.GetMesh()->GetPrimitiveType(), 0, 0,
		rs.GetMesh()->GetVertexCount(), 0, rs.GetMesh()->GetPrimitiveCount());

	rs.m_pRenderContext->GetEffect()->SetBool("g_bUseAtlas", false);
}