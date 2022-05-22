//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderSetSorted.cpp"															//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: To create a sorted set of renderable objects								//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
#include "CRenderSetSorted.h"
#include "CRenderContext.h"
#include "CRenderShape.h"
#include "../Mesh/CMesh.h"

CRenderSetSorted::CRenderSetSorted(void) : CRenderSet()
{
	m_pHead = 0;
	m_pTail = 0;
}

CRenderSetSorted::~CRenderSetSorted(void)
{
}

void CRenderSetSorted::AddRenderNode(CRenderNode *pNode)
{
	if(m_pHead == 0 && m_pTail == 0) // if it's 0
	{
		m_pHead = pNode;
		m_pTail = pNode;
		pNode->SetNext(0);
		return;
	}

	CRenderNode *iter = m_pHead;
	CRenderNode *prev = 0;
	while(iter)
	{
		if(SortFunc((CRenderShape*)iter, (CRenderShape*)pNode)) // add to the front
		{
			if(iter == m_pHead)
			{
				pNode->SetNext(m_pHead);
				m_pHead = pNode;
				//pNode->SetNext(0);
			}
			else
			{
				prev->SetNext(pNode);
				pNode->SetNext(iter);
			}
			return;
		}

		prev = iter;
		iter = iter->GetNext();
	}

	if(m_pHead == 0 && m_pTail == 0) // if it's 0
	{
		m_pHead = pNode;
		m_pTail = pNode;
		pNode->SetNext(0);
	}
	else
	{
		m_pTail->SetNext(pNode);
		m_pTail = pNode;
		pNode->SetNext(0);
	}
}

bool CRenderSetSorted::ZSortGreater(CRenderShape *left, CRenderShape *right)
{
	return left->GetZDepth() > right->GetZDepth();
}

bool CRenderSetSorted::ZSortSmaller(CRenderShape *left, CRenderShape *right)
{
	return left->GetZDepth() < right->GetZDepth();
}

bool CRenderSetSorted::TextureSortSmaller(CRenderShape *left, CRenderShape *right)
{
	return left->GetMesh()->GetTextureID() < right->GetMesh()->GetTextureID();
}