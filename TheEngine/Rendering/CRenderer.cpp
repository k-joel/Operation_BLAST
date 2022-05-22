#include "CRenderer.h"
#include "CRenderSet.h"
#include "Vertex.h"
#include "CRenderNode.h"

CRenderer CRenderer::m_Instance;

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}

CRenderer* CRenderer::GetInstance(void)
{
	return &m_Instance;
}

void CRenderer::Render(CRenderSet &set)
{
	CRenderNode *pNode = set.m_pHead;
	while(pNode)
	{
		pNode->RenderProcess();
		pNode = pNode->m_pNext;
	}
}