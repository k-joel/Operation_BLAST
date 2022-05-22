//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderStatic.h"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: An instance of a static mesh to be rendered. Has static functions to control//
//			the rendering properties and set the shader attributes.						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CRenderNode.h"
#include "../Mesh/CMesh.h"

class CRenderContext;
class CRenderStatic : public CRenderNode
{
protected:
	CRenderContext *m_pRenderContext;
	CMesh *m_pMesh;

	float m_fZDepth;

	int m_nDiffuseTexID;

	void *m_pData; // misc data
public:
	CRenderStatic();
	~CRenderStatic();

	void Init(CMesh *pMesh, CRenderContext* pRC, void (*RenderFunc)(CRenderNode&));

	CMesh *GetMesh() { return m_pMesh; }
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; m_nDiffuseTexID = pMesh->GetTextureID(); }

	float &GetZDepth() { return m_fZDepth; }
	void SetZDepth(float fDepth) { m_fZDepth = fDepth; }

	void *GetData() { return m_pData; }
	void SetData(void *pData) { m_pData = pData; }

	int &GetTexID() { return m_nDiffuseTexID; }
	void SetTexID(int nID) { m_nDiffuseTexID = nID; }

	CRenderContext *GetRenderContext() const { return m_pRenderContext; }
	void SetRenderContext(CRenderContext* pRC) { m_pRenderContext = pRC; }

	static void PCRenderFunc(CRenderNode &node);
	static void PNTRenderFunc(CRenderNode &node);
};