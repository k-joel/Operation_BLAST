#pragma once
#include <vector>
using std::vector;

#include "CEmitter.h"

class CParticleEffect
{
	friend class CEmitter;
	friend class CParticleManager;
private:
	string m_szName;
	bool m_bIsActive;

	vector<CEmitter*> m_Emitters;

public:
	CParticleEffect() :  m_szName(""), m_bIsActive(true) {}
	~CParticleEffect();

	void RunEmitterLoop(float fElapsedTime);

	bool GetIsActive() { return m_bIsActive; }
	void SetIsActive(bool bActive) { m_bIsActive = bActive; }
	void SetComplete(bool bComplete);

	void SetWorld(D3DXMATRIX world);
	void SetAttached(D3DXMATRIX *pAttach);
	void SetRotation(float fRot);

	bool CheckCollision(CBaseBV *pBV);

	vector<CEmitter*> &GetEmitterList() { return m_Emitters; }
};