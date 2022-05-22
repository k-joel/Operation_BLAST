//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CFrame.h"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Manages the frame hierarchy							 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx9math.h>
#include <vector>
using std::vector;

class CFrame
{
private:

	D3DXMATRIX m_mxWorld;
	D3DXMATRIX m_mxLocal;

	bool m_bDirty;
	CFrame *m_Parent;

public:
	CFrame(void);
	~CFrame(void);

	std::vector<CFrame*> m_Children;

	const D3DXMATRIX &GetWorldMatrix();
	D3DXMATRIX &GetLocalMatrix();

	void SetParent(CFrame *pParent)
	{
		m_Parent = pParent;
		m_Parent->m_Children.push_back(this);
	}

	void SetDirty(bool bDirty) { m_bDirty = bDirty; }
};