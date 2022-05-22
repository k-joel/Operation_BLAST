//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderSet.h"																	//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: To create a linked list of renderable objects								//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class CRenderNode;
class CRenderSet
{
	friend class CRenderer;
protected:
	CRenderNode *m_pHead;
	CRenderNode *m_pTail;

public:
	CRenderSet();
	~CRenderSet();

	virtual void AddRenderNode(CRenderNode *pNode);
	void ClearRenderSet();
	CRenderNode *GetHead() { return m_pHead; }
};