#pragma once
#include "../CBase.h"

class CFlag : public CBase
{
	float m_fLifeTime;
public:
	D3DXVECTOR3 m_vgoal;
	float m_fWinTimer;

	CFlag();
	~CFlag();

	void Initialize();
	void Update(float fElapsedTime);
};