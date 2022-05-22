//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderSetSorted.h"															//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: To create a sorted set of renderable objects								//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CRenderSet.h"
#include "CRenderShape.h"

class CRenderShape;
class CRenderSetSorted : public CRenderSet
{
public:
	CRenderSetSorted(void);
	~CRenderSetSorted(void);
	bool (*SortFunc)(CRenderShape *left, CRenderShape *right);
	void AddRenderNode(CRenderNode *pNode);

	static bool ZSortGreater(CRenderShape *left, CRenderShape *right);
	static bool ZSortSmaller(CRenderShape *left, CRenderShape *right);
	static bool TextureSortSmaller(CRenderShape *left, CRenderShape *right);
};