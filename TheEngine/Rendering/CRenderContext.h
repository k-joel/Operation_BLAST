//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderContext.h"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Sorts all renderable objects by their shader context						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CRenderNode.h"
#include "CDirect3D.h"
#include "CShaderManager.h"
#include "CRenderSetSorted.h"

class CRenderShape;
class CRenderContext : public CRenderNode
{
protected:

	void InitShaderHandles();

	CRenderSet *m_pRenderSet;

	int m_nLightMapID;

	LPD3DXEFFECT m_pEffect;

public:
	CRenderContext();
	~CRenderContext();

	// the effect needed by this context
	D3DXHANDLE m_hTechnique;
	D3DXHANDLE m_hSkinTech;

	D3DXHANDLE m_hViewProj;
	D3DXHANDLE m_hWorld;
	D3DXHANDLE m_hWorldView;
	D3DXHANDLE m_hProj;
	D3DXHANDLE m_hLight;
	D3DXHANDLE m_hMaterial;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hMatrixPalette;
	D3DXHANDLE m_hGamma;

	// particle handles
	D3DXHANDLE m_hEyePosL;
	D3DXHANDLE m_hViewportHeight;

	void SetTechnique(const char* szTechName);
	void SetWorld(const D3DXMATRIX &mxWorld);
	void SetWorldView(const D3DXMATRIX &mxWorldView);
	void SetProj(const D3DXMATRIX &mxProj);
	void SetViewProj(const D3DXMATRIX &mxVP);
	void SetLight(const TDirLight &tLight);
	void SetMaterial(const TMaterial &tMtrl);
	void SetTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetMatrixPalette(const D3DXMATRIX* mxPalette, const UINT nMatCount);
	void SetGamma(const float &fGamma);
	void SetEyePos(const D3DXVECTOR3 &vPos);
	void SetViewportHeight(const int nHeight);
	void CommitChanges();

	void CreateRenderSet(bool bSorted = false,
		bool (*SortFunc)(CRenderShape* left, CRenderShape* right) = 0);

	CRenderSet &GetRenderSet() { return *m_pRenderSet; }

	const LPD3DXEFFECT &GetEffect() { return m_pEffect; }
	void SetEffect(LPD3DXEFFECT pEffect)
	{
		m_pEffect = pEffect;
		InitShaderHandles();
	}

	void SetLightMapTexID(int nID) { m_nLightMapID = nID; }

	void AddRenderNode(	CRenderNode *pNode )
	{
		m_pRenderSet->AddRenderNode(pNode);
	}

	void ClearRenderSet()
	{
		m_pRenderSet->ClearRenderSet();
	}

	static void FlatRenderContextFunc(CRenderNode &node);
	static void SkinnedTexturedLitRenderContextFunc(CRenderNode &node);
	static void TexturedWVPRenderContextFunc(CRenderNode &node);
	static void TexturedLitRenderContextFunc(CRenderNode &node);
	static void TransparentTexturedLitRenderContextFunc(CRenderNode &node);
	static void WireframeRenderContextFunc(CRenderNode &node);
	static void ParticleRenderContextFunc(CRenderNode &node);
	static void HUDParticleRenderContextFunc(CRenderNode &node);
	static void TexUVScrollRenderContextFunc(CRenderNode &node);
	static void DepthRenderContextFunc(CRenderNode &node);
};