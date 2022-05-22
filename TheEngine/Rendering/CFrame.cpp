//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CFrame.cpp"																	//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: Manages the frame hierarchy							 						//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#include "CFrame.h"

CFrame::CFrame(void)
{
	m_bDirty = true;
	m_Parent = NULL;
	D3DXMatrixIdentity(&m_mxWorld);
	D3DXMatrixIdentity(&m_mxLocal);
}

CFrame::~CFrame(void)
{
}

const D3DXMATRIX &CFrame::GetWorldMatrix()
{
	if(m_bDirty)
	{
		if(m_Parent == NULL)
			m_mxWorld = m_mxLocal;
		else
			D3DXMatrixMultiply(&m_mxWorld, &m_mxLocal, &m_Parent->GetWorldMatrix());

		m_bDirty = false;
	}

	return m_mxWorld;
}

D3DXMATRIX &CFrame::GetLocalMatrix()
{
	m_bDirty = true;
	for(unsigned int i = 0; i < m_Children.size(); i++)
		m_Children[i]->SetDirty(true);

	return m_mxLocal;
}