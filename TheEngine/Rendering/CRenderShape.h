//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderShape.h"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: An instance of a transformed mesh to be rendered. Has static functions to 	//
//			control the rendering properties and set the shader attributes.				//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "..\Mesh\animation.h"
#include "CFrame.h"
#include "CRenderStatic.h"

struct TUVAtlasData
{
	float fRate;
	D3DXVECTOR2 vDimension;
};

struct TUVScrollData
{
	float fRate;
	D3DXVECTOR2 vDirection;
};

class CModel;
class CRenderContext;
class CRenderShape : public CRenderStatic
{
	CFrame m_Frame; // we need this if we need multiple models per object

	CModel *m_pModel;
	CAnimationControler m_pAC;

	TMaterial m_tMtrl;

public:
	CRenderShape();
	~CRenderShape();

	void Init(CMesh *pMesh, CModel* pModel, CRenderContext* pRC, D3DXMATRIX &mxWorld,
		void (*RenderFunc)(CRenderNode&));
	void Init(CMesh *pMesh, D3DXMATRIX &mxWorld, CRenderContext* pRC, void (*RenderFunc)(CRenderNode&));
	void Init(CModel *pModel, D3DXMATRIX &mxWorld, CRenderContext* pRC, void (*RenderFunc)(CRenderNode&));

	float m_fDelay;
	float m_fCurTime;

	CFrame &GetFrame() { return m_Frame; }

	CAnimationControler &GetAnimCtrl() { return m_pAC; }

	TMaterial &GetMaterial() { return m_tMtrl; }

	static void PCRenderFunc(CRenderNode &node);
	static void PNTRenderFunc(CRenderNode &node);
	static void ShadowPNTRenderFunc(CRenderNode &node);
	static void PNTTRenderFunc(CRenderNode &node);
	static void PNTBBRenderFunc(CRenderNode &node);
	static void ParticleRenderFunc(CRenderNode &node);
	static void DepthPNTRenderFunc(CRenderNode &node, CRenderNode &contextNode);
	static void UVScrollPNTRenderFunc(CRenderNode &node);
	static void UVAtlasPNTRenderFunc(CRenderNode &node);
};