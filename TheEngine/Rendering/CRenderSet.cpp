//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderSet.cpp"																//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: To create a linked list of renderable objects								//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
#include "CRenderSet.h"
#include "CRenderNode.h"

CRenderSet::CRenderSet()
{
	ClearRenderSet();
}

CRenderSet::~CRenderSet()
{
}

void CRenderSet::ClearRenderSet()
{
	m_pHead = 0;
	m_pTail = 0;
}

void CRenderSet::AddRenderNode(CRenderNode *pNode)
{
	if(m_pHead == 0 || m_pTail == 0)
	{
		m_pHead = pNode;
		m_pTail = pNode;
		m_pTail->SetNext(0);
	}
	else
	{
		m_pTail->SetNext(pNode);
		m_pTail = pNode;
		m_pTail->SetNext(0);
	}
}