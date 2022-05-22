//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderContext.cpp"															//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Sorts all renderable objects by their shader context						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#include "CRenderContext.h"
#include "CRenderContextManager.h"
#include "CRenderer.h"
#include "CRenderShape.h"

#include "../States/CGameState.h"
#include "../Core/CConfigLoader.h"
#include "../Camera/Camera.h"

CRenderContext::CRenderContext()
{
}

CRenderContext::~CRenderContext()
{
	delete m_pRenderSet;
}

void CRenderContext::CreateRenderSet(bool bSorted,
									 bool (*SortFunc)(CRenderShape* left, CRenderShape* right))
{
	if(bSorted)
	{
		m_pRenderSet = new CRenderSetSorted;
		((CRenderSetSorted *)m_pRenderSet)->SortFunc = SortFunc;
	}
	else
		m_pRenderSet = new CRenderSet;
}

void CRenderContext::InitShaderHandles()
{
	m_hTechnique = m_pEffect->GetTechniqueByName("RenderScene");

	m_hViewProj = m_pEffect->GetParameterByName(0, "g_mViewProj");
	m_hWorld = m_pEffect->GetParameterByName(0, "g_mWorld");
	m_hWorldView = m_pEffect->GetParameterByName(0, "g_mWorldView");
	m_hProj = m_pEffect->GetParameterByName(0, "g_mProj");
	m_hLight = m_pEffect->GetParameterByName(0, "g_tLight");
	m_hMaterial = m_pEffect->GetParameterByName(0, "g_mMaterial");
	m_hTexture = m_pEffect->GetParameterByName(0, "g_txScene");
	m_hMatrixPalette = m_pEffect->GetParameterByName(0, "amPalette");
	m_hGamma = m_pEffect->GetParameterByName(0, "g_fGamma");

	// particle handles
	m_hEyePosL = m_pEffect->GetParameterByName(0, "g_EyePosL");
	m_hViewportHeight = m_pEffect->GetParameterByName(0, "g_ViewportHeight");
}

// Shader params
void CRenderContext::SetTechnique(const char* szTechName)
{
	m_pEffect->SetTechnique(szTechName);
}

void CRenderContext::SetTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pEffect->SetTexture(m_hTexture, pTexture);
}

void CRenderContext::SetWorld(const D3DXMATRIX &mxWorld)
{
	m_pEffect->SetMatrix(m_hWorld, &mxWorld);
}

void CRenderContext::SetViewProj(const D3DXMATRIX &mxVP)
{
	m_pEffect->SetMatrix(m_hViewProj, &mxVP);
}

void CRenderContext::SetWorldView(const D3DXMATRIX &mxWorldView)
{
	m_pEffect->SetMatrix(m_hWorldView, &mxWorldView);
}

void CRenderContext::SetProj(const D3DXMATRIX &mxProj)
{
	m_pEffect->SetMatrix(m_hProj, &mxProj);
}

void CRenderContext::SetLight(const TDirLight &tLight)
{
	m_pEffect->SetValue(m_hLight, &tLight, sizeof(TDirLight));
}

void CRenderContext::SetMaterial(const TMaterial &tMtrl)
{
	m_pEffect->SetValue(m_hMaterial, &tMtrl, sizeof(TMaterial));
}

void CRenderContext::SetMatrixPalette(const D3DXMATRIX* mxPalette, const UINT nMatCount)
{
	m_pEffect->SetMatrixArray(m_hMatrixPalette, mxPalette, nMatCount);
}

void CRenderContext::SetGamma(const float &fGamma)
{
	m_pEffect->SetFloat(m_hGamma, fGamma);
}

void CRenderContext::SetEyePos(const D3DXVECTOR3 &vPos)
{
	m_pEffect->SetValue(m_hEyePosL, &vPos, sizeof(D3DXVECTOR3));
}

void CRenderContext::SetViewportHeight(const int nHeight)
{
	m_pEffect->SetInt(m_hViewportHeight, nHeight);
}

void CRenderContext::CommitChanges()
{
	m_pEffect->CommitChanges();
}

void CRenderContext::FlatRenderContextFunc(CRenderNode &node)
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.SetTechnique(rc.m_hTechnique);
	rc.SetViewProj(CCamera::GetInstance()->GetViewProjectionMatrix());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);
	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::SkinnedTexturedLitRenderContextFunc(CRenderNode &node)//
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.GetEffect()->SetTechnique(rc.m_hTechnique);
	rc.SetViewProj(CCamera::GetInstance()->GetViewProjectionMatrix());
	rc.SetLight(CRenderContextManager::GetInstance()->GetLight());
	rc.SetGamma(CRenderContextManager::GetInstance()->GetGamma());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);

	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::TexturedWVPRenderContextFunc(CRenderNode &node)//
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.SetTechnique(rc.m_hTechnique);
	rc.SetGamma(CRenderContextManager::GetInstance()->GetGamma());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);
	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::TexturedLitRenderContextFunc(CRenderNode &node)//
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.GetEffect()->SetTechnique(rc.m_hTechnique);
	rc.SetViewProj(CCamera::GetInstance()->GetViewProjectionMatrix());
	rc.SetLight(CRenderContextManager::GetInstance()->GetLight());
	rc.SetGamma(CRenderContextManager::GetInstance()->GetGamma());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);

	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::TransparentTexturedLitRenderContextFunc(CRenderNode &node)//
{
	CRenderContext &rc = (CRenderContext&)node;

	DWORD dwSrc;
	DWORD dwDest;

	CDirect3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	CDirect3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &dwDest);
	CDirect3D::GetInstance()->GetSprite()->Flush();
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	TexturedLitRenderContextFunc(node);

	CDirect3D::GetInstance()->GetSprite()->Flush();
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, dwDest);
}

void CRenderContext::WireframeRenderContextFunc(CRenderNode &node)//
{
	CRenderContext &rc = (CRenderContext&)node;

	CDirect3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	FlatRenderContextFunc(node);
	CDirect3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CRenderContext::ParticleRenderContextFunc(CRenderNode &node)
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.GetEffect()->SetTechnique(rc.m_hTechnique);
	rc.SetViewProj(CCamera::GetInstance()->GetViewProjectionMatrix());
	rc.SetEyePos(CCamera::GetInstance()->GetViewPosition());
	rc.SetViewportHeight(CConfigLoader::GetInstance()->GetResHeight());
	rc.SetGamma(CRenderContextManager::GetInstance()->GetGamma());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);

	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::HUDParticleRenderContextFunc(CRenderNode &node)
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.GetEffect()->SetTechnique(rc.m_hTechnique);
	rc.SetGamma(CRenderContextManager::GetInstance()->GetGamma());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);

	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::TexUVScrollRenderContextFunc( CRenderNode &node )
{
	CRenderContext &rc = (CRenderContext&)node;

	rc.GetEffect()->SetTechnique(rc.m_hTechnique);
	rc.SetViewProj(CCamera::GetInstance()->GetViewProjectionMatrix());
	rc.SetLight(CRenderContextManager::GetInstance()->GetLight());
	rc.SetGamma(CRenderContextManager::GetInstance()->GetGamma());

	rc.GetEffect()->SetFloat("g_fTime", CGameState::GetInstance()->GetGameTime());

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);

	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);
		CRenderer::GetInstance()->Render(*rc.m_pRenderSet);
		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}

void CRenderContext::DepthRenderContextFunc( CRenderNode &node )
{
	CRenderContext &rc = (CRenderContext&)node;

	CRenderContext *head = (CRenderContext*)rc.GetRenderSet().GetHead();

	rc.GetEffect()->SetTechnique(rc.m_hTechnique);

	UINT passes = 0;
	rc.GetEffect()->Begin(&passes, 0);

	for (UINT i = 0; i < passes; i++)
	{
		rc.GetEffect()->BeginPass(i);

		while(head)
		{
			CRenderNode *shapes = (CRenderNode*)head->GetRenderSet().GetHead();

			while(shapes)
			{
				CRenderShape::DepthPNTRenderFunc(*shapes, rc);
				//shapes->DepthRenderFunc(*shapes, rc);
				shapes = shapes->GetNext();
			}
			head = (CRenderContext*)head->GetNext();
		}

		rc.GetEffect()->EndPass();
	}
	rc.GetEffect()->End();
}