#pragma once

class CRenderNode
{
	friend class CRenderSet;
	friend class CRenderer;

protected:
	CRenderNode *m_pNext;

public:
	CRenderNode() {m_pNext = 0;}

	void (*RenderFunc)(CRenderNode &node);
	void (*DepthRenderFunc)(CRenderNode&, CRenderNode&);
	void RenderProcess() { RenderFunc(*this); }

	CRenderNode *GetNext() { return m_pNext; }
	void SetNext(CRenderNode *pNode) { m_pNext = pNode; }
};