#pragma once

#include "CDirect3D.h"

class CRenderSet;
class CRenderContext;
class CRenderer
{
	CRenderer(void);
	~CRenderer(void);

	static CRenderer m_Instance;

public:

	static CRenderer* GetInstance(void);

	void Render(CRenderSet &set);
};